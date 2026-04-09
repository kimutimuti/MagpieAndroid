#include "Effects.h"

namespace Effects {
    
    const std::string getFSRShader() {
        return R"(
            #version 300 es
            precision highp float;
            
            uniform sampler2D uTexture;
            uniform vec2 uResolution;
            uniform float uScale;
            
            in vec2 vTexCoord;
            out vec4 fragColor;
            
            vec4 APrxLoRcpF16(vec4 x) {
                return vec4(1.0) / x;
            }
            
            vec4 FsrEasuCF(vec2 xy, vec2 con) {
                return xy * con + 0.5;
            }
            
            vec4 FsrEasuSF(vec2 xy, vec2 con) {
                return xy * con - 0.5;
            }
            
            vec4 FsrEasuRF(vec4 p) {
                return p;
            }
            
            vec4 FsrEasuAG(vec4 a) {
                return a;
            }
            
            void main() {
                vec2 texCoord = vTexCoord;
                vec4 color = texture(uTexture, texCoord);
                
                vec2 pp = vec2(1.0) / uResolution;
                
                vec4 c = texture(uTexture, texCoord);
                vec4 tl = texture(uTexture, texCoord + vec2(-pp.x, -pp.y));
                vec4 t = texture(uTexture, texCoord + vec2(0.0, -pp.y));
                vec4 tr = texture(uTexture, texCoord + vec2(pp.x, -pp.y));
                vec4 l = texture(uTexture, texCoord + vec2(-pp.x, 0.0));
                vec4 r = texture(uTexture, texCoord + vec2(pp.x, 0.0));
                vec4 bl = texture(uTexture, texCoord + vec2(-pp.x, pp.y));
                vec4 b = texture(uTexture, texCoord + vec2(0.0, pp.y));
                vec4 br = texture(uTexture, texCoord + vec2(pp.x, pp.y));
                
                vec4 minRGB = min(min(min(tl, t), min(tr, l)), min(min(c, r), min(bl, min(b, br))));
                vec4 maxRGB = max(max(max(tl, t), max(tr, l)), max(max(c, r), max(bl, max(b, br))));
                
                vec4 result = c;
                
                float dtl = dot(c.rgb - tl.rgb, c.rgb - tl.rgb);
                float dt = dot(c.rgb - t.rgb, c.rgb - t.rgb);
                float dtr = dot(c.rgb - tr.rgb, c.rgb - tr.rgb);
                float dl = dot(c.rgb - l.rgb, c.rgb - l.rgb);
                float dr = dot(c.rgb - r.rgb, c.rgb - r.rgb);
                float dbl = dot(c.rgb - bl.rgb, c.rgb - bl.rgb);
                float db = dot(c.rgb - b.rgb, c.rgb - b.rgb);
                float dbr = dot(c.rgb - br.rgb, c.rgb - br.rgb);
                
                float minD = min(min(min(dtl, dt), min(dtr, dl)), min(min(dr, dbl), min(db, dbr)));
                
                if (minD == dtl) result = (c + tl) * 0.5;
                else if (minD == dt) result = (c + t) * 0.5;
                else if (minD == dtr) result = (c + tr) * 0.5;
                else if (minD == dl) result = (c + l) * 0.5;
                else if (minD == dr) result = (c + r) * 0.5;
                else if (minD == dbl) result = (c + bl) * 0.5;
                else if (minD == db) result = (c + b) * 0.5;
                else if (minD == dbr) result = (c + br) * 0.5;
                
                result = clamp(result, minRGB, maxRGB);
                fragColor = vec4(result.rgb, 1.0);
            }
        )";
    }
}
