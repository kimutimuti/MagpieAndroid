#include "Effects.h"

namespace Effects {
    
    const std::string getBicubicShader() {
        return R"(
            #version 300 es
            precision highp float;
            
            uniform sampler2D uTexture;
            uniform vec2 uResolution;
            uniform float uScale;
            
            in vec2 vTexCoord;
            out vec4 fragColor;
            
            float cubic(float x) {
                float x2 = x * x;
                float x3 = x2 * x;
                if (x &lt; 1.0) {
                    return (1.5 * x3 - 2.5 * x2 + 1.0);
                } else if (x &lt; 2.0) {
                    return (-0.5 * x3 + 2.5 * x2 - 4.0 * x + 2.0);
                }
                return 0.0;
            }
            
            vec4 bicubicSample(vec2 coord) {
                vec2 texelSize = 1.0 / uResolution * (1.0 / uScale);
                vec2 f = fract(coord * uResolution * uScale);
                coord -= f * texelSize;
                
                vec4 result = vec4(0.0);
                float totalWeight = 0.0;
                
                for (int i = -1; i &lt;= 2; i++) {
                    for (int j = -1; j &lt;= 2; j++) {
                        vec2 offset = vec2(float(i), float(j)) * texelSize;
                        float wx = cubic(abs(float(i) - f.x));
                        float wy = cubic(abs(float(j) - f.y));
                        float weight = wx * wy;
                        result += texture(uTexture, coord + offset) * weight;
                        totalWeight += weight;
                    }
                }
                
                return result / totalWeight;
            }
            
            void main() {
                fragColor = bicubicSample(vTexCoord);
            }
        )";
    }
}
