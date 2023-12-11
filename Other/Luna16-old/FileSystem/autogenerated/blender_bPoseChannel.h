/* Copyright (C) 2006 Charlie C
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/
// Auto generated from makesdna dna.c
#ifndef __BLENDER_BPOSECHANNEL__H__
#define __BLENDER_BPOSECHANNEL__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ListBase.h"

namespace Blender {


    // ---------------------------------------------- //
    class bPoseChannel
    {
    public:
        bPoseChannel *next;
        bPoseChannel *prev;
        ListBase constraints;
        char name[32];
        short flag;
        short constflag;
        short ikflag;
        short selectflag;
        short protectflag;
        short agrp_index;
        int pathlen;
        int pathsf;
        int pathef;
        Bone *bone;
        bPoseChannel *parent;
        bPoseChannel *child;
        ListBase iktree;
        void *b_bone_mats;
        void *dual_quat;
        void *b_bone_dual_quats;
        vec3f loc;
        vec3f size;
        vec4f quat;
        float chan_mat[4][4];
        float pose_mat[4][4];
        float constinv[4][4];
        vec3f pose_head;
        vec3f pose_tail;
        vec3f limitmin;
        vec3f limitmax;
        vec3f stiffness;
        float ikstretch;
        float *path;
        Object *custom;
    };
}


#endif//__BLENDER_BPOSECHANNEL__H__
