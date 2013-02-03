package net.geckotales.jtrace;

import java.util.*;
import javax.vecmath.*;

public class SimpleObject implements Object {
    Transformation transformation = new NullTransformation();
    Shape shape;
    SurfaceShader surfaceShader = null;
    VolumeShader volumeShader = null;
    Vector<SurfaceModifier> surfaceModifiers = new Vector<SurfaceModifier>();
    
    public SimpleObject(Shape shape) {
        this.shape = shape;
    }
    
    public SimpleObject(Shape shape, Transformation transformation) {
        this.shape = shape;
        this.transformation = transformation;
    }
    
    public boolean inside(Point3d p, double t) {
        // if (transformation != null)
        //   newP = p * inverse
        //   return shape.inside(newP)
        // else
        // return shape.inside(p)
        
        if (transformation != null) {
            Point3d newP = new Point3d(p);            
            transformation.transform(newP);
            
            return shape.inside(p, t);
        } else {
            return shape.inside(p, t);
        }
    }
    
    //public boolean intersects(Ray r) {
    //}
    
    public Set<Intersection> intersect(Ray r) {
        // if (transformation != null)
        //   newRay = ray * inverse
        //   intersections = shape.intersect(r)
        //   for each intersection in intersections
        //     global position = intersection.position * transformation
        // else
        //   intersections = shape.intersect(r)
        //
        // return intersections
        Set<Intersection> ret = null;
        
        if (transformation != null) {
            Point3d newOrigin = new Point3d(r.getOrigin());
            Vector3d newDirection = new Vector3d(r.getDirection());
            
            transformation.inverse().transform(newOrigin);
            transformation.inverse().transformNormal(newDirection);
            
            Ray newRay = new Ray(newOrigin, newDirection);

            ret = shape.intersect(newRay);

            for (Intersection i: ret) {
                i.setObject(this);
                i.setGlobalPosition(i.getPosition());
                i.setGlobalNormal(i.getNormal());
                transformation.transform(i.getGlobalPosition(), r.getTime());
                transformation.transformNormal(i.getGlobalNormal(), r.getTime());
            }
        } else {
            ret = shape.intersect(r);

            for (Intersection i: ret) {
                i.setObject(this);
                i.setGlobalPosition(i.getPosition());
                i.setGlobalNormal(i.getNormal());
            }
        }
        
        return ret;            
    }
    
    public SurfaceShader getSurfaceShader() {
        return surfaceShader;
    }

    public VolumeShader getVolumeShader() {
        return volumeShader;
    }
    
    public Vector<SurfaceModifier> getSurfaceModifiers() {
        return surfaceModifiers;
    }

    public void setSurfaceShader(SurfaceShader shader) {
        surfaceShader = shader;
    }
    
    public void setVolumeShader(VolumeShader shader) {
        volumeShader = shader;
    }
    
    public void addSurfaceModifier(SurfaceModifier modifier) {
        surfaceModifiers.add(modifier);
    }
}