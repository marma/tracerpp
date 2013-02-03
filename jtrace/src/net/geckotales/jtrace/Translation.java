package net.geckotales.jtrace;

import javax.vecmath.*;

public class Translation implements Transformation {
    Vector3d translation;
    Transformation inverse = null;
    
    public Translation(double x, double y, double z) {
        this.translation = new Vector3d(x,y,z);
    }
    
    public Translation(Vector3d translation) {
        this.translation = new Vector3d(translation);
    }
    
    public Transformation inverse() {
        if (inverse == null) {
            inverse = new Translation(-translation.x, -translation.y, -translation.z);
        }
        
        return inverse;
    }
    
    public void transform(Point3d p) {
        p.x += translation.x;
        p.y += translation.y;
        p.z += translation.z;
    }
    
    public void transform(Vector3d v) {
    }

    public void transformNormal(Vector3d v) {
    }
    
    public void transform(Point3d p, double t) {
        transform(p);
    }
    
    public void transform(Vector3d v, double t) {
        transform(v);
    }
    
    public void transformNormal(Vector3d v, double t) {        
        transformNormal(v);
    }
}
