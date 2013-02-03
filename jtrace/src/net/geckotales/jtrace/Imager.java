package net.geckotales.jtrace;

import java.io.*;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;

public class Imager {
    double inlevels_min = 0, inlevels_max = 1, levels_pow = 1, outlevels_min = 0, outlevels_max = 1, dither = 0;
    
    public int getARGB(Color c) {
        Color x = process(c);
        
        return ((int)(255) << 24) + ((int)(255*x.r) << 16) + ((int)(255*x.g) << 8) + (int)(255*x.b);
    }
    
    public Color process(Color c) {
        Color ret = new Color(new Color(1,1,1).sub(c.exp(-2))).map(outlevels_min, outlevels_max, inlevels_min, inlevels_max).pow(levels_pow).clamp(0,1);
        
        if (ret.r > 1 || ret.g > 1 || ret.b > 1) ret.r = ret.g = ret.b = 0;
        
        return ret;
    }
    
    public void setInLevels(double min, double max) {
        inlevels_min = min;
        inlevels_max = max;
    }
    
    public void setOutLevels(double min, double max) {
        outlevels_min = min;
        outlevels_max = max;
    }
    
    public void setGamma(double gamma) {
        levels_pow = gamma;
    }
}
