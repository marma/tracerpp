package net.geckotales.jtrace.lightsources;

import net.geckotales.jtrace.*;
import javax.vecmath.*;

public class SphereLightsource implements Lightsource {
    Point3d origin;
    Color c;
    double radius = 1.0;
    
    public SphereLightsource(Point3d p, double r, Color c) {
        origin = new Point3d(p);
        radius = r;
        this.c = c;
    }
    
    public Ray createRay(ShadeContext shadeContext) {
        double dx = Math.random(), dy = Math.random(), dz = Math.random();
        
        while (dx*dx + dy*dy + dz*dz > 1.0) {
            dx = Math.random();
            dy = Math.random();
            dz = Math.random();
        }
        
        Point3d newOrigin = new Point3d(origin.x + radius*dx, origin.y + radius*dy, origin.z + radius*dz);
        Vector3d direction = new Vector3d(shadeContext.getGlobalPosition());
        direction.sub(newOrigin);
        direction.normalize();
        
        Ray ray = new Ray();
        ray.setOrigin(newOrigin);
        ray.setDirection(direction);
        ray.setColor(c);
        ray.setTime(shadeContext.getTime());
        
        return ray;
    }
    
    public boolean isDistributed() {
        return true;
    }
}