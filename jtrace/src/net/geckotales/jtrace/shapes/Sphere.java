package net.geckotales.jtrace.shapes;

import java.util.*;
import javax.vecmath.*;
import net.geckotales.jtrace.*;

public class Sphere implements Shape {
    Point3d origin = new Point3d();
    double radius = 1.0, radius2 = 1.0;
    
    public Sphere() {
    }
    
    public Sphere(Point3d o, double r) {
        origin.set(o);
        radius = r;
        radius2 = r*r;
    }
    
    public boolean inside(Point3d p, double t) {
        Vector3d v = new Vector3d(origin);
        v.sub(p);
        
        return v.lengthSquared() <= radius;
    }
    
    public Set<Intersection> intersect(Ray r) {
        Set<Intersection> intersections = new HashSet<Intersection>();
        
        double X0 = r.getOrigin().x, Y0 = r.getOrigin().y, Z0 = r.getOrigin().z,
        Xd = r.getDirection().x, Yd = r.getDirection().y, Zd = r.getDirection().z,
        Xc = origin.x, Yc = origin.y, Zc = origin.z,
        B = 2 * (Xd * (X0 - Xc) + Yd * (Y0 - Yc) + Zd * (Z0 - Zc)),
        C = (X0 - Xc)*(X0 - Xc) + (Y0 - Yc)*(Y0 - Yc) + (Z0 - Zc)*(Z0 - Zc) - radius2,
        disc = B*B - 4*C;
        
        if (disc<0) {
            return intersections;
        } else {
            double t0 = (-B - Math.sqrt(disc))/2, t1 = (-B + Math.sqrt(disc))/2;
            if (t0 > t1) { double tmp=t0;t0=t1;t1=tmp; }
            
            if (t0 > 0) {
                Vector3d p = new Vector3d(r.getOrigin());
                Vector3d v = new Vector3d(r.getDirection());
                v.scale(t0);
                p.add(v);                
                Intersection i = new Intersection();                
                i.setType(Intersection.IN);
                i.setPosition(p);                
                p.sub(origin);
                p.normalize();                
                i.setNormal(p);       
                i.setSortValue(t0);
                
                intersections.add(i);
            }
            
            if (t1 > 0) {
                Vector3d p = new Vector3d(r.getOrigin());
                Vector3d v = new Vector3d(r.getDirection());
                v.scale(t1);
                p.add(v);                
                Intersection i = new Intersection();                
                i.setType(Intersection.OUT);
                i.setPosition(p);                
                p.sub(origin);
                p.normalize();                
                i.setNormal(p);                
                i.setSortValue(t1);
                
                intersections.add(i);
            }
        }
        
        return intersections;
    }
}
