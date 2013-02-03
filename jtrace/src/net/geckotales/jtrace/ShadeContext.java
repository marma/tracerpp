package net.geckotales.jtrace;

import java.util.*;
import javax.vecmath.*;

public class ShadeContext {
    Transformation transformation = null;
    Point3d position = new Point3d(), globalPosition = new Point3d();
    Vector3d normal = new Vector3d(), globalNormal = new Vector3d();
    Scene scene = null;
    double time = 0.0;

    public ShadeContext(Transformation t, Intersection i, Scene s) {
        transformation = t;
        position.set(i.getPosition());
        globalPosition.set(i.getGlobalPosition());
        normal.set(i.getNormal());
        globalNormal.set(i.getGlobalNormal());
        scene = s;
    }
    
    public Transformation getTransformation() {
        return transformation;
    }
    
    public Scene getScene() {
        return scene;
    }
    
    public Point3d getPosition() {
        return position;
    }
    
    public Point3d getGlobalPosition() {
        return globalPosition;
    }
    
    public Vector3d getNormal() {
        return normal;
    }
    
    public Vector3d getGlobalNormal() {
        return globalNormal;
    }
    
    public double getTime() {
        return time;
    }
    
    public void setTransformation(Transformation t) {
        transformation = t;
    }
    
    public void setScene(Scene scene) {
        this.scene = scene;
    }
    
    public void setPosition(Point3d p) {
        position.set(p);
        globalPosition.set(p);
        transformation.transform(globalPosition, time);
    }
    
    public void setGlobalPosition(Point3d p) {
        globalPosition.set(p);
        position.set(p);
        transformation.inverse().transform(position, time);
    }
    
    public void setNormal(Vector3d n) {
        normal.set(n);
        globalPosition.set(n);
        transformation.transformNormal(globalNormal, time);
    }
    
    public void setGlobalNormal(Vector3d n) {
        globalNormal.set(n);
        normal.set(n);
        transformation.inverse().transformNormal(normal, time);
    }
    
    public void setTime(double t) {
        time = t;
    }
}
