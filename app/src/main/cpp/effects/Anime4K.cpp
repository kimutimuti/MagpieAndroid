#include "Effects.h"

namespace Effects {
    
    const std::string getAnime4KShader() {
        return R"(
            #version 300 es
            precision highp float;
            
            uniform sampler2D uTexture;
            uniform vec2 uResolution;
            uniform float uScale;
            
            in vec2 vTexCoord;
            out vec4 fragColor;
            
            #define GET(x, y) texture(uTexture, vTexCoord + vec2(float(x), float(y)) / uResolution * (1.0 / uScale))
            
            vec4 Anime4K_Upscale(vec2 coord) {
                vec4 c = GET(0, 0);
                vec4 n = GET(0, -1);
                vec4 s = GET(0, 1);
                vec4 w = GET(-1, 0);
                vec4 e = GET(1, 0);
                
                vec4 mn = min(min(n, s), min(w, e));
                vec4 mx = max(max(n, s), max(w, e));
                
                return clamp(c, mn, mx);
            }
            
            void main() {
                vec4 color = Anime4K_Upscale(vTexCoord);
                
                vec4 c = texture(uTexture, vTexCoord);
                vec4 n = texture(uTexture, vTexCoord + vec2(0.0, -1.0) / uResolution);
                vec4 s = texture(uTexture, vTexCoord + vec2(0.0, 1.0) / uResolution);
                vec4 w = texture(uTexture, vTexCoord + vec2(-1.0, 0.0) / uResolution);
                vec4 e = texture(uTexture, vTexCoord + vec2(1.0, 0.0) / uResolution);
                
                vec4 mn = min(min(min(c, n), min(s, w)), e);
                vec4 mx = max(max(max(c, n), max(s, w)), e);
                
                vec4 result = clamp(color, mn, mx);
                fragColor = vec4(result.rgb, 1.0);
            }
        )";
    }
}
