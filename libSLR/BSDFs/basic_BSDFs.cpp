//
//  basic_BSDFs.cpp
//
//  Created by 渡部 心 on 2015/09/05.
//  Copyright (c) 2015年 渡部 心. All rights reserved.
//

#include "basic_BSDFs.h"
#include "../Core/distributions.h"

namespace SLR {
    SampledSpectrum LambertianBRDF::sampleInternal(const BSDFQuery &query, const BSDFSample &smp, BSDFQueryResult* result) const {
        result->dir_sn = cosineSampleHemisphere(smp.uDir[0], smp.uDir[1]);
        result->dirPDF = result->dir_sn.z / M_PI;
        result->dirType = m_type;
        result->dir_sn.z *= query.dir_sn.z > 0 ? 1 : -1;
        return m_R / M_PI;
    }
    
    SampledSpectrum LambertianBRDF::evaluateInternal(const BSDFQuery &query, const Vector3D &dir) const {
        return m_R / M_PI;
    }
    
    float LambertianBRDF::evaluatePDFInternal(const BSDFQuery &query, const Vector3D &dir) const {
        return query.dir_sn.z * dir.z >= 0.0f ? std::abs(dir.z) / M_PI : 0.0f;
    }
    
    float LambertianBRDF::weightInternal(const BSDFQuery &query, const BSDFSample &smp) const {
#ifdef Use_BSDF_Actual_Weights
        return m_R.maxValue();
#else
        return m_R.maxValue();
#endif
    }
    
    float LambertianBRDF::weightInternal(const BSDFQuery &query, const Vector3D &dir) const {
#ifdef Use_BSDF_Actual_Weights
        return m_R.maxValue();
#else
        return m_R.maxValue();
#endif
    }
    
    SampledSpectrum LambertianBRDF::getBaseColorInternal(DirectionType flags) const {
        return m_R;
    }
    
    SampledSpectrum SpecularBRDF::sampleInternal(const BSDFQuery &query, const BSDFSample &smp, BSDFQueryResult* result) const {
        result->dir_sn = Vector3D(-query.dir_sn.x, -query.dir_sn.y, query.dir_sn.z);
        result->dirPDF = 1.0f;
        result->dirType = m_type;
        SampledSpectrum ret = m_coeffR * m_fresnel->evaluate(query.dir_sn.z) / std::fabs(query.dir_sn.z);
        BSDF_SAMPLE_ASSERT;
        return ret;
    }
    
    SampledSpectrum SpecularBRDF::evaluateInternal(const BSDFQuery &query, const Vector3D &dir) const {
        return SampledSpectrum::Zero;
    }
    
    float SpecularBRDF::evaluatePDFInternal(const BSDFQuery &query, const Vector3D &dir) const {
        return 0.0f;
    }
    
    float SpecularBRDF::weightInternal(const BSDFQuery &query, const BSDFSample &smp) const {
#ifdef Use_BSDF_Actual_Weights
        BSDFQueryResult result;
        float fs = sample(query, smp, &result).maxValue();
        return fs * std::fabs(result.dir_sn.z) / result.dirPDF;
#else
        return m_coeffR.maxValue();
#endif
    }
    
    float SpecularBRDF::weightInternal(const BSDFQuery &query, const Vector3D &dir) const {
        return 0.0f;
    }
    
    SampledSpectrum SpecularBRDF::getBaseColorInternal(DirectionType flags) const {
        return m_coeffR;
    }
    
    SampledSpectrum SpecularBTDF::sampleInternal(const BSDFQuery &query, const BSDFSample &smp, BSDFQueryResult* result) const {
        bool entering = query.dir_sn.z > 0.0f;
        float eEnter = entering ? m_fresnel.etaExt()[query.wlHint] : m_fresnel.etaInt()[query.wlHint];
        float eExit = entering ? m_fresnel.etaInt()[query.wlHint] : m_fresnel.etaExt()[query.wlHint];
        
        float sinEnter2 = 1.0f - query.dir_sn.z * query.dir_sn.z;
        float rrEta = eEnter / eExit;// reciprocal of relative IOR.
        float sinExit2 = rrEta * rrEta * sinEnter2;
        
        if (sinExit2 >= 1.0f) {
            result->dirPDF = 0.0f;
            return SampledSpectrum::Zero;
        }
        float cosExit = std::sqrt(std::fmax(0.0f, 1.0f - sinExit2));
        if (entering)
            cosExit = -cosExit;
        result->dir_sn = Vector3D(rrEta * -query.dir_sn.x, rrEta * -query.dir_sn.y, cosExit);
        result->dirPDF = 1.0f;
        result->dirType = m_type;
        cosExit = std::fabs(cosExit);
        float F = FresnelDielectric::evalF(eEnter, eExit, std::fabs(query.dir_sn.z), cosExit);
        SampledSpectrum ret = SampledSpectrum::Zero;
        ret[query.wlHint] = m_coeffT[query.wlHint] * (1.0f - F) / cosExit;
        BSDF_SAMPLE_ASSERT;
        return ret;
    }
    
    SampledSpectrum SpecularBTDF::evaluateInternal(const BSDFQuery &query, const Vector3D &dir) const {
        return SampledSpectrum::Zero;
    }
    
    float SpecularBTDF::evaluatePDFInternal(const BSDFQuery &query, const Vector3D &dir) const {
        return 0.0f;
    }
    
    float SpecularBTDF::weightInternal(const BSDFQuery &query, const BSDFSample &smp) const {
#ifdef Use_BSDF_Actual_Weights
        BSDFQueryResult result;
        float fs = sample(query, smp, &result)[query.wlHint];
        return result.dirPDF > 0.0f ? fs * std::fabs(result.dir_sn.z) / result.dirPDF : 0.0f;
#else
        return m_coeffT.maxValue();
#endif
    }
    
    float SpecularBTDF::weightInternal(const BSDFQuery &query, const Vector3D &dir) const {
        return 0.0f;
    }
    
    SampledSpectrum SpecularBTDF::getBaseColorInternal(DirectionType flags) const {
        return m_coeffT;
    }
    
    SampledSpectrum InverseBSDF::sampleInternal(const BSDFQuery &query, const BSDFSample &smp, BSDFQueryResult* result) const {
        BSDFQuery mQuery = query;
        mQuery.flags = mQuery.flags.flip();
        SampledSpectrum ret = m_baseBSDF->sample(mQuery, smp, result);
        result->dirType = result->dirType.flip();
        result->dir_sn.z *= -1;
        return ret;
    }
    
    SampledSpectrum InverseBSDF::evaluateInternal(const BSDFQuery &query, const Vector3D &dir) const {
        BSDFQuery mQuery = query;
        mQuery.flags = mQuery.flags.flip();
        Vector3D mDir = dir;
        mDir.z *= -1;
        return m_baseBSDF->evaluate(mQuery, mDir);
    }
    
    float InverseBSDF::evaluatePDFInternal(const BSDFQuery &query, const Vector3D &dir) const {
        BSDFQuery mQuery = query;
        mQuery.flags.flip();
        Vector3D mDir = dir;
        mDir.z *= -1;
        return m_baseBSDF->evaluatePDF(mQuery, mDir);
    }
    
    float InverseBSDF::weightInternal(const BSDFQuery &query, const BSDFSample &smp) const {
        BSDFQuery mQuery = query;
        mQuery.flags = mQuery.flags.flip();
        return m_baseBSDF->weight(mQuery, smp);
    }
    
    float InverseBSDF::weightInternal(const BSDFQuery &query, const Vector3D &dir) const {
        BSDFQuery mQuery = query;
        mQuery.flags = mQuery.flags.flip();
        Vector3D mDir = dir;
        mDir.z *= -1;
        return m_baseBSDF->weight(mQuery, mDir);
    }
    
    SampledSpectrum InverseBSDF::getBaseColorInternal(DirectionType flags) const {
        return m_baseBSDF->getBaseColor(flags.flip());
    }
}
