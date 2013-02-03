package net.geckotales.jtrace.lightsources;

import net.geckotales.jtrace.*;
import javax.vecmath.*;

public class SampledAmbientLightsource implements AmbientLightsource {
    Color c;
    double radius = 100;
    int nSamples = 4;
    
    public SampledAmbientLightsource(int nSamples, Color c) {
        this.nSamples = nSamples;
        this.c = c;
    }
    
    public SampledAmbientLightsource(Color c) {
        this.c = c;
    }
    
    public Color ambient(ShadeContext shadeContext) {
        Color ret = new Color(0,0,0);
        double scale = (-Math.acos(shadeContext.getGlobalNormal().y)/Math.PI + 1.0)/2.0;
        
        for (int i=0;i<nSamples;i++) {
            double dx = 2*Math.random()-1, dy = Math.random(), dz = 2*Math.random()-1;
            
            while (dx*dx + dy*dy + dz*dz > 1.0) {
                dx = 2*Math.random() - 1;
                dy = Math.random();
                dz = 2*Math.random() - 1;
            }
            
            double s = Math.sqrt(dx*dx + dy*dy + dz*dz);
            Point3d x = new Point3d(dx/s, dy/s, dz/s);
            Vector3d direction = new Vector3d(-dx/s, -dy/s, -dz/s);
            x.scale(radius);
            x.add(shadeContext.getGlobalPosition());
            
            Ray r = new Ray(x, direction);
            r.setColor(c);
            r.setTime(shadeContext.getTime());
            
            ret.add(JTrace.shadow(r, shadeContext.getGlobalPosition(), shadeContext.getScene()));
        }
        
        if (nSamples != 0) {
            ret.mul(scale/nSamples);
        } else {
            ret.set(scale, scale, scale);
        }
        
        return ret;
    }
}