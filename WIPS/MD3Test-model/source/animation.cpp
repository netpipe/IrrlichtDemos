#include <os.h>
#include <fast_atof.h>
#include "animation.h"

namespace irr
{
namespace scene
{
namespace quake3
{

static core::stringc AANIMATION_NAME[MAX_TOTALANIMATIONS] =
    {
        "BOTH_DEATH1",
        "BOTH_DEAD1",
        "BOTH_DEATH2",
        "BOTH_DEAD2",
        "BOTH_DEATH3",
        "BOTH_DEAD3",
        "TORSO_GESTURE",
        "TORSO_ATTACK",
        "TORSO_ATTACK2",
        "TORSO_DROP",
        "TORSO_RAISE",
        "TORSO_STAND",
        "TORSO_STAND2",
        "LEGS_WALKCR",
        "LEGS_WALK",
        "LEGS_RUN",
        "LEGS_BACK",
        "LEGS_SWIM",
        "LEGS_JUMP",
        "LEGS_LAND",
        "LEGS_JUMPB",
        "LEGS_LANDB",
        "LEGS_IDLE",
        "LEGS_IDLECR",
        "LEGS_TURN",
        "TORSO_GETFLAG",
        "TORSO_GUARDBASE",
        "TORSO_PATROL",
        "TORSO_FOLLOWME",
        "TORSO_AFFIRMATIVE",
        "TORSO_NEGATIVE",
        "MAX_ANIMATIONS",
        "LEGS_BACKCR",
        "LEGS_BACKWALK",
        "FLAG_RUN",
        "FLAG_STAND",
        "FLAG_STAND2RUN"
    };


s32 Animation::find(const c8 * name) const
{
    core::stringc search;
    search = name;
    search.make_upper();

    u32 i;
    for (i=0; i < MAX_TOTALANIMATIONS; i++) {
         if (AANIMATION_NAME[i] == search)
             return (s32) i;
    }

    return -1;
}


bool parseFootSteps(Script* script, EMD3_FOOTSTEP_TYPE & FootSteps)
{
    while ( script->nextToken() != ETK_TOKEN_EOF )
    {
       switch ( script->token().getType() )
       {
           case ETK_TOKEN_TOKEN:
           {
               const core::stringc & value = script->token().getValue();

                    if ( value == "default" ) { FootSteps = FOOTSTEP_NORMAL; return true; }
               else if ( value == "normal"  ) { FootSteps = FOOTSTEP_NORMAL; return true; }
               else if ( value == "boot"    ) { FootSteps = FOOTSTEP_BOOT;   return true; }
               else if ( value == "flesh"   ) { FootSteps = FOOTSTEP_FLESH;  return true; }
               else if ( value == "mech"    ) { FootSteps = FOOTSTEP_MECH;   return true; }
               else if ( value == "energy"  ) { FootSteps = FOOTSTEP_ENERGY; return true; }
               else
               { // Bad token.
                   os::Printer::log ("Wrong \"footstep\" token in animation file, in line", core::stringc( script->line() ).c_str(), ELL_ERROR);
                   return false;
               }
           }

           case ETK_TOKEN_EOL: // Empty lines.
              continue;

           default:
           {
              os::Printer::log ("Wrong \"footstep\" token type in skin file, in line", core::stringc( script->line() ).c_str(), ELL_ERROR);
              return false;
           }

        } // switch.

    } // while.

    os::Printer::log ("Premature end in \"footstep\" token in skin file, in line", core::stringc( script->line() ).c_str(), ELL_ERROR);
    return false;
}

bool parseHeadOffset(Script* script, core::vector3df & HeadOffset)
{
    u32 i;
    f32 headoffset[3];

    i = 0;

    while ( script->nextToken() != ETK_TOKEN_EOF )
    {
       switch ( script->token().getType() )
       {
           case ETK_TOKEN_TOKEN:
           {
               const core::stringc & value = script->token().getValue();

               if (i < 3) {
                  f32 f;
                  const c8 * r;

                  r = core::fast_atof_move(value.c_str(), f);

                  if ( (u32)(r - value.c_str()) == value.size() ) {
                      headoffset[i++] = f;
                      if (i == 3) {
                         HeadOffset.X = headoffset[0];
                         HeadOffset.Y = headoffset[1];
                         HeadOffset.Z = headoffset[2];

                         return true; // It is a valid headoffset.s
                      }
                  }
                  else {
                     os::Printer::log ("Wrong float value for \"headoffset\" token in animation file, in line", core::stringc( script->line() ).c_str(), ELL_ERROR);
                     return false;
                  }
               }
               else {
                  os::Printer::log ("Too many float value for \"headoffset\" token in animation file, in line", core::stringc( script->line() ).c_str(), ELL_ERROR);
                  return false;
               }
           }

           case ETK_TOKEN_EOL: // Empty lines.
              continue;

           default:
           {
              os::Printer::log ("Wrong \"headoffset\" token type in skin file, in line", core::stringc( script->line() ).c_str(), ELL_ERROR);
              return false;
           }

        } // switch.

    } // while.

    os::Printer::log ("Premature end in \"headoffset\" token in skin file, in line", core::stringc( script->line() ).c_str(), ELL_ERROR);
    return false;
}

bool parseSex(Script* script, EMD3_SEX_TYPE & Sex)
{
    while ( script->nextToken() != ETK_TOKEN_EOF )
    {
       switch ( script->token().getType() )
       {
           case ETK_TOKEN_TOKEN:
           {
               const core::stringc & value = script->token().getValue();

                    if ( value == "f" || value == "F" ) { Sex = GENDER_FEMALE; }
               else if ( value == "n" || value == "N" ) { Sex = GENDER_NEUTER; }
               else    { Sex = GENDER_MALE; }

               return true;
           }

           case ETK_TOKEN_EOL: // Empty lines.
              continue;

           default:
           {
              os::Printer::log ("Wrong \"sex\" token type in skin file, in line", core::stringc( script->line() ).c_str(), ELL_ERROR);
              return false;
           }

        } // switch.

    } // while.

    os::Printer::log ("Premature end in \"sex\" token in skin file, in line", core::stringc( script->line() ).c_str(), ELL_ERROR);
    return false;
}


bool parseAnimations(Script* script, SMD3_Animation *Animations)
{
    u32 cntAnim, cntItem, skip;
    cntAnim = 0;
    cntItem = 0;
    skip    = 0;

    script->setAllowLineBreaks(false);

    do
    {
       switch ( script->token().getType() )
       {
           case ETK_TOKEN_TOKEN:
           {
               const core::stringc & value = script->token().getValue();
               f32 f;
               const c8 * r;

               r = core::fast_atof_move(value.c_str(), f);

               if ( (u32)(r - value.c_str()) != value.size() ) {
                  os::Printer::log ("Wrong number value for \"headoffset\" token in animation file, in line", core::stringc( script->line() ).c_str(), ELL_ERROR);
                  return false;
               }

               switch (cntItem++)
               {
                   case 0:
                   {
                      Animations[cntAnim].firstFrame = (s32) f;
                   // leg only frames are adjusted to not count the upper body only frames
                      if ( cntAnim == LEGS_WALKCR )
                         skip = Animations[LEGS_WALKCR].firstFrame - Animations[TORSO_GESTURE].firstFrame;
                      else if ( cntAnim >= LEGS_WALKCR && cntAnim < TORSO_GETFLAG )
                         Animations[cntAnim].firstFrame -= skip;
                      break;
                   }
                   case 1:
                   {
                       Animations[cntAnim].numFrames = (s32) f;

                       Animations[cntAnim].reversed = false;
                    // if numFrames is negative the animation is reversed
                       if (Animations[cntAnim].numFrames < 0)
                       {
                           Animations[cntAnim].numFrames = -Animations[cntAnim].numFrames;
                           Animations[cntAnim].reversed = true;
                       }
                       break;
                   }
                   case 2:
                   {
                       Animations[cntAnim].loopFrames = (s32) f;
                       break;
                   }
                   case 3:
                   {
                       Animations[cntAnim].fps = (s32) f;
                       if (Animations[cntAnim].fps == 0)
                           Animations[cntAnim].fps = 1;
                       Animations[cntAnim].frameLerp = (s32) truncf((float) 1000 / Animations[cntAnim].fps);
                       Animations[cntAnim].initialLerp = (s32) truncf((float) 1000 / Animations[cntAnim].fps);
                       break;
                   }
                   default:
                   {
                       os::Printer::log ("Too many animations parameters in animation file, in line", core::stringc( script->line() ).c_str(), ELL_ERROR);
                       return false;
                   }
               }
               break;
           }

           case ETK_TOKEN_EOF:
           {
              if (cntItem == 4)
              {
                  cntAnim++;
                  cntItem = 0;
              }
              if ( cntAnim >= TORSO_GETFLAG && cntItem == 0 )
              {
                 Animations[cntAnim].firstFrame  = Animations[TORSO_GESTURE].firstFrame;
                 Animations[cntAnim].frameLerp   = Animations[TORSO_GESTURE].frameLerp;
                 Animations[cntAnim].fps         = Animations[TORSO_GESTURE].fps;
                 Animations[cntAnim].initialLerp = Animations[TORSO_GESTURE].initialLerp;
                 Animations[cntAnim].loopFrames  = Animations[TORSO_GESTURE].loopFrames;
                 Animations[cntAnim].numFrames   = Animations[TORSO_GESTURE].numFrames;
                 Animations[cntAnim].reversed    = false;
                 cntAnim++;
                 break;
              }
              os::Printer::log ("The number of types of animations are less than expected", ELL_ERROR);
              return false;
           }

           case ETK_TOKEN_EOL: // New lines.
           {
              if (cntItem == 4)
              {
                  cntAnim++;
                  cntItem = 0;
                  break;
              }
              os::Printer::log ("Missing parameters in the animation of the line", core::stringc( script->line() ).c_str(), ELL_ERROR);
              return false;
           }

           default:
           {
              os::Printer::log ("Wrong \"animation\" token type in skin file, in line", core::stringc( script->line() ).c_str(), ELL_ERROR);
              return false;
           }

        } // switch.

        script->nextToken();

    } while ( cntAnim < MAX_ANIMATIONS );

 // crouch backward animation
    memcpy(&Animations[LEGS_BACKCR], &Animations[LEGS_WALKCR], sizeof(SMD3_Animation));
    Animations[LEGS_BACKCR].reversed = true;
 // walk backward animation
    memcpy(&Animations[LEGS_BACKWALK], &Animations[LEGS_WALK], sizeof(SMD3_Animation));
    Animations[LEGS_BACKWALK].reversed = true;
 // flag moving fast
    Animations[FLAG_RUN].firstFrame = 0;
    Animations[FLAG_RUN].numFrames = 16;
    Animations[FLAG_RUN].loopFrames = 16;
    Animations[FLAG_RUN].frameLerp = 1000 / 15;
    Animations[FLAG_RUN].initialLerp = 1000 / 15;
    Animations[FLAG_RUN].fps = 15;
    Animations[FLAG_RUN].reversed = false;
 // flag not moving or moving slowly
    Animations[FLAG_STAND].firstFrame = 16;
    Animations[FLAG_STAND].numFrames = 5;
    Animations[FLAG_STAND].loopFrames = 0;
    Animations[FLAG_STAND].frameLerp = 1000 / 20;
    Animations[FLAG_STAND].initialLerp = 1000 / 20;
    Animations[FLAG_STAND].fps = 20;
    Animations[FLAG_STAND].reversed = false;
 // flag speeding up
    Animations[FLAG_STAND2RUN].firstFrame = 16;
    Animations[FLAG_STAND2RUN].numFrames = 5;
    Animations[FLAG_STAND2RUN].loopFrames = 1;
    Animations[FLAG_STAND2RUN].frameLerp = 1000 / 15;
    Animations[FLAG_STAND2RUN].initialLerp = 1000 / 15;
    Animations[FLAG_STAND2RUN].fps = 15;
    Animations[FLAG_STAND2RUN].reversed = true;

 // Ok!
    return true;
}

void Animation::parse()
{
 // Initial value.
    Sex = GENDER_NEUTER;
    FootSteps = FOOTSTEP_NORMAL;
    FixedLegs = false;
    FixedTorso = false;
    memset(Animations, 0, sizeof(SMD3_Animation)*MAX_TOTALANIMATIONS);

 // Init parse.
    getScript()->setAllowLineBreaks(true);

    while ( getScript()->nextToken() != ETK_TOKEN_EOF )
    {
       switch ( getScript()->token().getType() )
       {
           case ETK_TOKEN_TOKEN: // If
           {
              const core::stringc & value = getScript()->token().getValue();

           // Checking token.
              if ( value == "footsteps") { if ( parseFootSteps(getScript(), FootSteps) ) continue; }
              else if ( value == "headoffset" ) { if ( parseHeadOffset(getScript(), HeadOffset) ) continue; }
              else if ( value == "sex" ) { if ( parseSex(getScript(), Sex) ) continue; }
              else if ( value == "fixedlegs" ) { FixedLegs = true; continue; }
              else if ( value == "fixedtorso" ) { FixedTorso = true; continue; }
              else if ( value[0] >= '0' && value[0] <= '9' ) { if ( parseAnimations(getScript(), Animations) ) continue; }
              else // Bad token.
                 os::Printer::log ("Wrong token in skin file, in line", core::stringc( getScript()->line() ).c_str(), ELL_ERROR);

              return;
           }

           case ETK_TOKEN_EOL: // Empty lines.
              continue;

           default:
           {
              os::Printer::log ("Wrong token type in skin file, in line", core::stringc( getScript()->line() ).c_str(), ELL_ERROR);
              return;
           }

       } // switch ( ... )

    } // while ( ... )
}

} // end namespace quake3
} // end namespace scene
} // end namespace irr


