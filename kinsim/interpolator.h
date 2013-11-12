//
//  interpolator.h
//  kinsim
//
//  Created by Rene on 12/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#ifndef kinsim_interpolator_h
#define kinsim_interpolator_h

struct outpath{
    double* jointpos1;
    double* jointpos2;
    double* jointpos3;
    int length;
};

struct outpath interpol();

#endif
