package net.geckotales.jtrace;

import javax.vecmath.*;

public class NullTransformation implements Transformation {
    public Transformation inverse() {
        return this;
    }
    
    public void transform(Point3d p) {
    }
    
    public void transform(Vector3d v) {
    }

    public void transformNormal(Vector3d v) {
    }
    
    public void transform(Point3d p, double t) {
    }
    
    public void transform(Vector3d v, double t) {
    }
    
    public void transformNormal(Vector3d v, double t) {        
    }
}
