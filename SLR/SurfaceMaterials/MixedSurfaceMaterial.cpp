//
//  MixedSurfaceMaterial.cpp
//
//  Created by 渡部 心 on 2015/09/06.
//  Copyright (c) 2015年 渡部 心. All rights reserved.
//

#include "MixedSurfaceMaterial.h"
#include "../BSDFs/MultiBSDF.h"
#include "../Memory/ArenaAllocator.h"
#include "../Core/textures.h"

BSDF* MixedSurfaceMaterial::getBSDF(const SurfacePoint &surfPt, ArenaAllocator &mem, float scale) const {
    float factor = m_factor->evaluate(surfPt.texCoord);
    BSDF* bsdf0 = m_mat0->getBSDF(surfPt, mem, scale * (1.0f - factor));
    BSDF* bsdf1 = m_mat1->getBSDF(surfPt, mem, scale * factor);
    MultiBSDF* bsdf = mem.create<MultiBSDF>();
    bsdf->add(bsdf0);
    bsdf->add(bsdf1);
    return bsdf;
}
