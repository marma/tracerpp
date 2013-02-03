package net.geckotales.jtrace;

import javax.vecmath.*;

public class Camera {
    Transformation transformation = null;
    double fStop = 1.4, focalLength = 24.0/1000.0, shutterSpeed = 1/100, focalDistance = 5;
    boolean stochastic = false;
    
    public Camera() {
        transformation = new NullTransformation();
    }
    
    public Camera(Transformation transformation) {
        this.transformation = transformation;
    }
    
    public Ray getRay(double x, double y, double t) {
        Point3d origin = new Point3d(0,0,0);
        Vector3d direction = new Vector3d();
        
        direction.x = x/focalLength;
        direction.y = y/focalLength;
        direction.z = -1;
        direction.normalize();                
        
        if (stochastic) {
            double dx = Math.random(), dy = Math.random();

            while (dx*dx + dy*dy > 1) {
                dx = Math.random();
                dy = Math.random();            
            }
            
            double apertureArea = focalLength/fStop, apertureRadius = Math.sqrt(apertureArea/3.14159);
            Point3d focPoint = new Point3d(direction);
            focPoint.scale(Math.abs(focalDistance/focPoint.z));
            Point3d newOrigin = new Point3d(apertureRadius*dx, apertureRadius*dy, 0);
            Vector3d newDirection = new Vector3d(focPoint.x - newOrigin.x, focPoint.y - newOrigin.y, focPoint.z - newOrigin.z);
            newDirection.normalize();
            
            origin = newOrigin;
            direction = newDirection;
        }
        
        transformation.transform(origin);
        transformation.transformNormal(direction);
        
        return new Ray(origin, direction, t);
    }
    
    public Transformation getTransformation() {
        return transformation;
    }
    
    public void setTransformation(Transformation transformation) {
        this.transformation = transformation;
    }
    
    public boolean getStochastic() {
        return stochastic;
    }
    
    public void setStochastic(boolean s) {
        stochastic = s;
    }
    
    public void setAperture(double aperture) {
        fStop = aperture;
    }
    
    public void setFocalDistance(double fd) {
        focalDistance = fd;
    }
    
    public void setFocalLength(double fl) {
        focalLength = fl/1000.0;
    }
    
    public void setShutterSpeed(double ss) {
        shutterSpeed = ss;
    }
}
