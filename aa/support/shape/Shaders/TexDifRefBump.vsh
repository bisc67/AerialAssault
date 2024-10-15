;------------------------------------------------------------------------------
; Textured, diffuse, reflect, du dv bump vertex shader
;------------------------------------------------------------------------------
#include "D3D_Shape.hpp"


vs.1.0


;------------------------------------------------------------------------------
; Position calculation
;------------------------------------------------------------------------------

; Transform position to the clipping space
m4x4 oPos, VS_VERT_POS, VS_L2C_MATRIX


;------------------------------------------------------------------------------
; Lighting calculation
;------------------------------------------------------------------------------

; Do the lighting calculation
dp3 r1.x, VS_VERT_NORMAL, VS_LIGHT_DIR  ; r1 = normal dot light
max r1.x, r1.x, VS_ZERO                 ; if dot < 0 then dot = 0
mul r0, r1.x, VS_LIGHT_DIFFUSE_COL      ; Multiply with diffuse
add r0, r0.xyz, VS_LIGHT_AMBIENT_COL    ; Add in ambient
mov r0.w, VS_LIGHT_AMBIENT_COL.w        ; Use alpha from ambient color
min oD0, r0, VS_TWO                     ; clamp if > 2

;------------------------------------------------------------------------------
; Texture co-ord calculation
;------------------------------------------------------------------------------

mov oT0.xy, VS_VERT_UV                     ; Diffuse = vert uv
mov oT1.xy, VS_VERT_UV                     ; Bump    = vert uv
m4x4 oT2.xy, VS_VERT_NORMAL, VS_REF_MATRIX    ; Ref     = normal * ref matrix

;------------------------------------------------------------------------------
; Fog calculation
;------------------------------------------------------------------------------

; Just use constant fog color
mov oFog.x, VS_FOG_COL.w




