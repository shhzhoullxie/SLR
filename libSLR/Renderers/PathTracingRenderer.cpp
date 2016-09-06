//
//  PathTracingRenderer.cpp
//
//  Created by 渡部 心 on 2015/07/11.
//  Copyright (c) 2015年 渡部 心. All rights reserved.
//

#include "PathTracingRenderer.h"

#include "../Core/RenderSettings.h"
#include "../Helper/ThreadPool.h"
#include "../RNGs/XORShiftRNG.h"
#include "../Memory/ArenaAllocator.h"
#include "../Core/ImageSensor.h"
#include "../Core/RandomNumberGenerator.h"
#include "../Core/cameras.h"
#include "../Core/geometry.h"
#include "../Core/SurfaceObject.h"
#include "../Core/directional_distribution_functions.h"
#include "../Core/light_path_samplers.h"

namespace SLR {
    PathTracingRenderer::PathTracingRenderer(uint32_t spp) : m_samplesPerPixel(spp) {
        
    }
    
    void PathTracingRenderer::render(const Scene &scene, const RenderSettings &settings) const {
#ifdef DEBUG
        uint32_t numThreads = 1;
#else
        uint32_t numThreads = std::thread::hardware_concurrency();
#endif
        XORShiftRNG topRand(settings.getInt(RenderSettingItem::RNGSeed));
        std::unique_ptr<ArenaAllocator[]> mems = std::unique_ptr<ArenaAllocator[]>(new ArenaAllocator[numThreads]);
        std::unique_ptr<IndependentLightPathSampler[]> samplers = std::unique_ptr<IndependentLightPathSampler[]>(new IndependentLightPathSampler[numThreads]);
        for (int i = 0; i < numThreads; ++i) {
            new (mems.get() + i) ArenaAllocator();
            new (samplers.get() + i) IndependentLightPathSampler(topRand.getUInt());
        }
        std::unique_ptr<IndependentLightPathSampler*[]> samplerRefs = std::unique_ptr<IndependentLightPathSampler*[]>(new IndependentLightPathSampler*[numThreads]);
        for (int i = 0; i < numThreads; ++i)
            samplerRefs[i] = &samplers[i];
        
        const Camera* camera = scene.getCamera();
        ImageSensor* sensor = camera->getSensor();
        
        Job job;
        job.scene = &scene;
        
        job.mems = mems.get();
        job.pathSamplers = samplerRefs.get();
        
        job.camera = camera;
        job.timeStart = settings.getFloat(RenderSettingItem::TimeStart);
        job.timeEnd = settings.getFloat(RenderSettingItem::TimeEnd);
        
        job.sensor = sensor;
        job.imageWidth = settings.getInt(RenderSettingItem::ImageWidth);
        job.imageHeight = settings.getInt(RenderSettingItem::ImageHeight);
        job.numPixelX = sensor->tileWidth();
        job.numPixelY = sensor->tileHeight();
        
        uint32_t exportPass = 1;
        uint32_t imgIdx = 0;
        uint32_t endIdx = 16;
        
        sensor->init(job.imageWidth, job.imageHeight);
        
        std::chrono::system_clock::time_point start, end;
        start = std::chrono::system_clock::now();
        
        for (int s = 0; s < m_samplesPerPixel; ++s) {
            ThreadPool threadPool(numThreads);
            for (int ty = 0; ty < sensor->numTileY(); ++ty) {
                for (int tx = 0; tx < sensor->numTileX(); ++tx) {
                    job.basePixelX = tx * sensor->tileWidth();
                    job.basePixelY = ty * sensor->tileHeight();
                    threadPool.enqueue(std::bind(&Job::kernel, job, std::placeholders::_1));
                }
            }
            threadPool.wait();
            
            if ((s + 1) == exportPass) {
                char filename[256];
                sprintf(filename, "%03u.bmp", imgIdx);
                end = std::chrono::system_clock::now();
                double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                sensor->saveImage(filename, settings.getFloat(RenderSettingItem::Brightness) / (s + 1));
                printf("%u samples: %s, %g[s]\n", exportPass, filename, elapsed * 0.001f);
                ++imgIdx;
                if (imgIdx == endIdx)
                    break;
                exportPass += exportPass;
            }
        }
        
        //    sensor.saveImage("output.png", settings.getFloat(RenderSettingItem::SensorResponse) / numSamples);
    }
    
    void PathTracingRenderer::Job::kernel(uint32_t threadID) {
        ArenaAllocator &mem = mems[threadID];
        IndependentLightPathSampler &pathSampler = *pathSamplers[threadID];
        for (int ly = 0; ly < numPixelY; ++ly) {
            for (int lx = 0; lx < numPixelX; ++lx) {
                float time = pathSampler.getTimeSample(timeStart, timeEnd);
                PixelPosition p = pathSampler.getPixelPositionSample(basePixelX + lx, basePixelY + ly);
                
                float selectWLPDF;
                WavelengthSamples wls = WavelengthSamples::sampleUniform(pathSampler.getWavelengthSample(), &selectWLPDF);
                // Each component of wlPDFs is a probability density composed of
                // the probability density to sample each wavelength and the probability to select the wavelength as the hero wavelength,
                // the probability density to sample a path based on the hero wavelength.
                SampledSpectrum wlPDFs = selectWLPDF / WavelengthSamples::NumComponents;
                
                LensPosQuery lensQuery(time, wls);
                LensPosQueryResult lensResult;
                SampledSpectrum We0 = camera->sample(lensQuery, pathSampler.getLensPosSample(), &lensResult);
                wlPDFs *= lensResult.areaPDF;
                
                IDFSample WeSample(p.x / imageWidth, p.y / imageHeight);
                IDFQueryResult WeResult;
                IDF* idf = camera->createIDF(lensResult.surfPt, wls, mem);
                SampledSpectrum We1 = idf->sample(WeSample, &WeResult);
                wlPDFs *= WeResult.dirPDF;
                
                Ray ray(lensResult.surfPt.p, lensResult.surfPt.shadingFrame.fromLocal(WeResult.dirLocal), time);
                SampledSpectrum C = contribution(*scene, wls, wlPDFs, ray, pathSampler, mem);
                SLRAssert(C.hasNaN() == false && C.hasInf() == false && C.hasMinus() == false,
                          "Unexpected value detected: %s\n"
                          "pix: (%f, %f)", C.toString().c_str(), px, py);
                
                SampledSpectrum weight = We0 * We1 * absDot(ray.dir, lensResult.surfPt.gNormal) / wlPDFs[wls.heroIdx];
                SLRAssert(weight.hasNaN() == false && weight.hasInf() == false && weight.hasMinus() == false,
                          "Unexpected value detected: %s\n"
                          "pix: (%f, %f)", weight.toString().c_str(), px, py);
                sensor->add(p.x, p.y, wls, weight * C);
                
                mem.reset();
            }
        }
    }
    
    SampledSpectrum PathTracingRenderer::Job::contribution(const Scene &scene, const WavelengthSamples &wls, const SampledSpectrum &initWLPDFs,
                                                           const Ray &initRay, IndependentLightPathSampler &pathSampler, ArenaAllocator &mem) const {
        Ray ray = initRay;
        SurfacePoint surfPt;
        SampledSpectrum alpha = SampledSpectrum::One;
        SampledSpectrum wlPDFs = initWLPDFs;
        SampledSpectrum initY = alpha;
        SampledSpectrumSum sp(SampledSpectrum::Zero);
        uint32_t pathLength = 0;
        
        Intersection isect;
        if (!scene.intersect(ray, &isect))
            return SampledSpectrum::Zero;
        isect.getSurfacePoint(&surfPt);
        
        Vector3D dirOut_sn = surfPt.shadingFrame.toLocal(-ray.dir);
        if (surfPt.isEmitting()) {
            EDF* edf = surfPt.createEDF(wls, mem);
            SampledSpectrum Le = surfPt.emittance(wls) * edf->evaluate(EDFQuery(wls.heroIdx), dirOut_sn);
            FloatSum denomMISWeight = 0.0f;
            for (int i = 0; i < WavelengthSamples::NumComponents; ++i)
                denomMISWeight += wlPDFs[i];
            float MISWeight = wlPDFs[wls.heroIdx] / denomMISWeight;
            sp += MISWeight * alpha * Le;
        }
        if (surfPt.atInfinity)
            return sp;

        while (true) {
            ++pathLength;
            if (pathLength >= 100)
                break;
            Normal3D gNorm_sn = surfPt.shadingFrame.toLocal(surfPt.gNormal);
            BSDF* bsdf = surfPt.createBSDF(wls, mem);
            BSDFQuery fsQuery(wls.heroIdx, dirOut_sn, gNorm_sn);
            
            // Next Event Estimation (explicit light sampling)
            if (bsdf->hasNonDelta()) {
                float lightProb;
                Light light;
                scene.selectLight(pathSampler.getLightSelectionSample(), &light, &lightProb);
                SLRAssert(!std::isnan(lightProb) && !std::isinf(lightProb), "lightProb: unexpected value detected: %f", lightProb);
                
                LightPosQuery lpQuery(ray.time, wls);
                LightPosQueryResult lpResult;
                SampledSpectrum M = light.sample(lpQuery, pathSampler.getLightPosSample(), &lpResult);
                SLRAssert(!lpResult.areaPDF.hasNaN()/* && !std::isinf(xpResult.areaPDF)*/, "areaPDF: unexpected value detected: %s", lpResult.areaPDF.toString().c_str());
                
                if (scene.testVisibility(surfPt, lpResult.surfPt, ray.time)) {
                    float dist2;
                    Vector3D shadowDir = lpResult.surfPt.getDirectionFrom(surfPt.p, &dist2);
                    Vector3D shadowDir_l = lpResult.surfPt.shadingFrame.toLocal(-shadowDir);
                    Vector3D shadowDir_sn = surfPt.shadingFrame.toLocal(shadowDir);
                    
                    EDF* edf = lpResult.surfPt.createEDF(wls, mem);
                    SampledSpectrum Le = M * edf->evaluate(EDFQuery(wls.heroIdx), shadowDir_l);
                    SampledSpectrum lightPDFs = lightProb * lpResult.areaPDF * wlPDFs;
                    SLRAssert(!Le.hasNaN() && !Le.hasInf(), "Le: unexpected value detected: %s", Le.toString().c_str());
                    
                    SampledSpectrum fs = bsdf->evaluate(fsQuery, shadowDir_sn);
                    float cosLight = absDot(-shadowDir, lpResult.surfPt.gNormal);
                    SampledSpectrum bsdfPDFs = bsdf->evaluatePDF(fsQuery, shadowDir_sn) * wlPDFs * (cosLight / dist2);
                    
                    FloatSum denomMISWeight = 0.0f;
                    for (int i = 0; i < WavelengthSamples::NumComponents; ++i) {
                        denomMISWeight += lightPDFs[i] * lightPDFs[i];
                        if (!lpResult.posType.isDelta() && !lpResult.areaPDF.hasInf())
                            denomMISWeight += bsdfPDFs[i] * bsdfPDFs[i];
                    }
                    float MISWeight = lightPDFs[wls.heroIdx] * lightPDFs[wls.heroIdx] / denomMISWeight;
                    if (!std::isnan(MISWeight)) {
                        SLRAssert(MISWeight >= 0.0f && MISWeight <= 1.0f, "Invalid MIS weight: %g", MISWeight);
                        
                        float G = absDot(shadowDir_sn, gNorm_sn) * cosLight / dist2;
                        sp += alpha * Le * fs * (G * MISWeight / (lightProb * lpResult.areaPDF[wls.heroIdx]));
                        SLRAssert(!std::isnan(G) && !std::isinf(G), "G: unexpected value detected: %f", G);
                    }
                }
            }
            
            // get a next direction by sampling BSDF.
            BSDFQueryResult fsResult;
            SampledSpectrum fs = bsdf->sample(fsQuery, pathSampler.getBSDFSample(), &fsResult);
            if (fs == SampledSpectrum::Zero || fsResult.dirPDF[wls.heroIdx] == 0.0f)
                break;
            alpha *= fs * absDot(fsResult.dir_sn, gNorm_sn) / fsResult.dirPDF[wls.heroIdx];
            SLRAssert(!alpha.hasInf() && !alpha.hasNaN(),
                      "alpha: %s\nlength: %u, cos: %g, dirPDF: %s",
                      alpha.toString().c_str(), pathLength, absDot(fsResult.dir_sn, gNorm_sn), fsResult.dirPDF.toString().c_str());
            
            Vector3D dirIn = surfPt.shadingFrame.fromLocal(fsResult.dir_sn);
            ray = Ray(surfPt.p, dirIn, ray.time, Ray::Epsilon);
            
            // find a next intersection point.
            isect = Intersection();
            if (!scene.intersect(ray, &isect))
                break;
            isect.getSurfacePoint(&surfPt);
            
            dirOut_sn = surfPt.shadingFrame.toLocal(-ray.dir);
            
            // implicit light sampling
            if (surfPt.isEmitting()) {
                SampledSpectrum bsdfPDFs = fsResult.dirPDF * wlPDFs;
                
                EDF* edf = surfPt.createEDF(wls, mem);
                SampledSpectrum Le = surfPt.emittance(wls) * edf->evaluate(EDFQuery(wls.heroIdx), dirOut_sn);
                float lightProb = scene.evaluateProb(Light(isect.obj));
                float dist2 = surfPt.getSquaredDistance(ray.org);
                SampledSpectrum lightPDFs = lightProb * surfPt.evaluateAreaPDF() * wlPDFs * (dist2 / absDot(ray.dir, surfPt.gNormal));
                SLRAssert(!Le.hasNaN() && !Le.hasInf(), "Le: unexpected value detected: %s", Le.toString().c_str());
                SLRAssert(!lightPDFs.hasNaN()/* && !std::isinf(lightPDF)*/, "lightPDFs: %s", lightPDFs.toString().c_str());
                
                FloatSum denomMISWeight = 0.0f;
                for (int i = 0; i < WavelengthSamples::NumComponents; ++i) {
                    if (!fsResult.dirType.isDelta())
                        denomMISWeight += lightPDFs[i] * lightPDFs[i];
                    denomMISWeight += bsdfPDFs[i] * bsdfPDFs[i];
                }
                float MISWeight = bsdfPDFs[wls.heroIdx] * bsdfPDFs[wls.heroIdx] / denomMISWeight;
                if (!std::isnan(MISWeight)) {
                    SLRAssert(MISWeight >= 0.0f && MISWeight <= 1.0f, "Invalid MIS weight: %g", MISWeight);
                    sp += alpha * Le * MISWeight;
                }
            }
            if (surfPt.atInfinity)
                break;
            
            // Russian roulette
            // FIXME: ?? alpha at this point is wavelength-dependent value. Does this cause bias?
            SampledSpectrum continueProbs = min(alpha / initY, SampledSpectrum::One);
            if (pathSampler.getPathTerminationSample() >= continueProbs[wls.heroIdx])
                break;
            alpha /= continueProbs[wls.heroIdx];
            wlPDFs *= fsResult.dirPDF * continueProbs;
        }
        
        return sp;
    }
}
