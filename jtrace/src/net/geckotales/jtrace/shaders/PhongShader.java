package net.geckotales.jtrace.shaders;

import javax.vecmath.*;
import net.geckotales.jtrace.*;

public class PhongShader extends SurfaceShader {
    Color color = new Color(1,1,1);
    double kA = 1.0, kD = 0.75, kS = 1.0, phongSize = 25;
    
    public PhongShader() {
    }
    
    public PhongShader(Color c, double kA, double kD, double kS, double ps) {
        this.color.set(c);
        this.kA = kA;
        this.kD = kD;
        this.kS = kS;
        this.phongSize = ps;
    }
    
    public ShadeReturn shadeDirectional(Ray e, Ray l, ShadeContext shadeContext) {
        ShadeReturn ret = new ShadeReturn();
        double d = -shadeContext.getGlobalNormal().dot(l.getDirection());
        
        //d = Math.abs(d);
        
        if (d >= 0) {
            // diffuse
            Color diffuse = Color.mul(kD*d, Color.mul(color, l.getColor()));            
            ret.getColor().add(diffuse);
            
            Vector3d N = shadeContext.getGlobalNormal();
            Vector3d L = l.getDirection();
            Vector3d V = e.getDirection();
            Vector3d R = new Vector3d(N);
            R.scale(2*N.dot(L));
            R.sub(L);
            R.normalize();
            double d2 = R.dot(V);
            
            if (d2 > 0) {
                Color specular = Color.mul(kS*Math.pow(d2, phongSize), l.getColor());
                ret.getColor().add(specular);
            }
        }
        
        return ret;
    }
    
    public Color shadeAmbient(Color ambient, ShadeContext shadeContext) {
        return Color.mul(kA, Color.mul(ambient, color));
    }    
}