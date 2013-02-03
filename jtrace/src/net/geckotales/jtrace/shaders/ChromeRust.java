package net.geckotales.jtrace.shaders;

import javax.vecmath.*;
import net.geckotales.jtrace.*;
import net.geckotales.jtrace.util.*;

public class ChromeRust implements SurfaceModifier {
    public final static int NOISE   = 1;
    public final static int FBM     = 2;
    public final static int VLNOISE = 3;
    public final static int VLFBM   = 4;
    double scale = 4.0, bump_scale = 0.25, H = 2, lacunarity = 2, octaves = 8, d = 0.25, bump_level = 0.0;
    int type = VLNOISE;

    public void modify(ShadeContext shadeContext) {
        Point3d pos = new Point3d(shadeContext.getPosition());
        pos.scale(scale);
        Vector3d n = new Vector3d(shadeContext.getGlobalNormal());
        Vector3d noise = null;
        double d = 0;
                
        switch (type) {
            case (NOISE) : d = Noise.noise(pos); break;
            case (FBM) : d = Noise.fBm(pos, H, lacunarity, octaves); break;
            case (VLNOISE) : d = Noise.VLnoise(pos, d); break;
            case (VLFBM) : d = Noise.VLfBm(pos, d, H, lacunarity, octaves); break;
        }
        
        if (d > 0) {        
            switch (type) {
                case (NOISE) : noise = Noise.dnoise(pos); break;
                case (FBM) : noise = Noise.dfBm(pos, H, lacunarity, octaves); break;
                case (VLNOISE) : noise = Noise.dVLnoise(pos, d); break;
                case (VLFBM) : noise = Noise.dVLfBm(pos, d, H, lacunarity, octaves); break;
            }

            noise.scale(bump_scale);
            n.add(noise);
            n.normalize();
            shadeContext.setGlobalNormal(n);
        }
    }
}
