package net.geckotales.jtrace.shapes;

import java.util.*;
import javax.vecmath.*;
import net.geckotales.jtrace.*;

public class Plane implements Shape {
    Point3d origin = new Point3d();
    Vector3d normal = new Vector3d();
    
    public Plane() {
    }
    
    public Plane(Point3d o, Vector3d n) {
        origin.set(o);
        normal.set(n);
    }
    
    public boolean inside(Point3d p, double t) {
        // @todo fix this ...
        return false;
    }
    
    public Set<Intersection> intersect(Ray r) {
        Set<Intersection> intersections = new HashSet<Intersection>();

        // v_scal_n, ray.v scalar n
        double v_scal_n = r.getDirection().dot(normal);
 
        // Test if ray intersects plane at all
        if (v_scal_n<-0.0001) {
            // S1, distance to plane
            Vector3d tmp1 = new Vector3d(r.getOrigin());
            tmp1.sub(origin);
            
            double S1 = normal.dot(tmp1);
            // P2, ray.p projected onto plane
            // t, distance from ray.p to intersection, I
            double t = S1/v_scal_n;
            // I, intersection
            Vector3d tmp2 = new Vector3d(r.getDirection());
            tmp2.scale(-t);
            tmp2.add(r.getOrigin());
            
            // add I to IntersectionCollection icoll
            Intersection i = new Intersection();
            i.setPosition(tmp2);
            i.setNormal(normal);
            i.setSortValue(-t);
            i.setType(Intersection.IN);
            
            intersections.add(i);
            //icoll.insert(icoll.begin(), tracer::intersection(I, n, -t, r.t, tracer::intersection::ENTER)); 
        }
                
        return intersections;
    }
}
