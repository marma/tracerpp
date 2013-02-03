package net.geckotales.jtrace.lightsources;

import net.geckotales.jtrace.*;
import javax.vecmath.*;

public class PointLightsource implements Lightsource {
    Point3d origin;
    Color c;
    
    public PointLightsource(Point3d p, Color c) {
        origin = new Point3d(p);
        this.c = c;
    }
    
    public Ray createRay(ShadeContext shadeContext) {
        Vector3d direction = new Vector3d(shadeContext.getGlobalPosition());
        direction.sub(origin);
        direction.normalize();
        
        Ray ray = new Ray();
        ray.setOrigin(origin);
        ray.setDirection(direction);
        ray.setColor(c);
        ray.setTime(shadeContext.getTime());
        
        return ray;
    }
    
    public boolean isDistributed() {
        return false;
    }
}