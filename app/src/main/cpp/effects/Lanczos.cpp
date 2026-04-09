#include "Effects.h"

namespace Effects {
    
    const std::string getLanczosShader() {
        return R"(
            #version 300 es
            precision highp float;
            
            uniform sampler2D uTexture;
            uniform vec2 uResolution;
            uniform float uScale;
            
            in vec2 vTexCoord;
            out vec4 fragColor;
            
            const float PI = 3.14159265359;
            const int LANCZOS_SIZE = 3;
            
            float sinc(float x) {
                if (abs(x) &lt; 0.0001) return 1.0;
                return sin(PI * x) / (PI * x);
            }
            
            float lanczos(float x) {
                if (abs(x) &gt;= float(LANCZOS_SIZE)) return 0.0;
                return sinc(x) * sinc(x / float(LANCZOS_SIZE));
            }
            
            vec4 lanczosSample(vec2 coord) {
                vec2 texelSize = 1.0 / uResolution * (1.0 / uScale);
                vec2 f = fract(coord * uResolution * uScale);
                coord -= f * texelSize;
                
                vec4 result = vec4(0.0);
                float totalWeight = 0.0;
                
                for (int i = -LANCZOS_SIZE + 1; i &lt;= LANCZOS_SIZE; i++) {
                    for (int j = -LANCZOS_SIZE + 1; j &lt;= LANCZOS_SIZE; j++) {
                        vec2 offset = vec2(float(i), float(j)) * texelSize;
                        float wx = lanczos(float(i) - f.x);
                        float wy = lanczos(float(j) - f.y);
                        float weight = wx * wy;
                        result += texture(uTexture, coord + offset) * weight;
                        totalWeight += weight;
                    }
                }
                
                return result / totalWeight;
            }
            
            void main() {
                fragColor = lanczosSample(vTexCoord);
            }
        )";
    }
}
