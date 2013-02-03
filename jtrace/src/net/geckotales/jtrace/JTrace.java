package net.geckotales.jtrace;

import java.io.*;
import java.util.*;
import javax.imageio.ImageIO;
import javax.vecmath.*;
import java.awt.image.BufferedImage;

import net.geckotales.jtrace.shapes.*;
import net.geckotales.jtrace.shaders.*;
import net.geckotales.jtrace.lightsources.*;

public class JTrace {
    public static Color shadow(Ray r, Point3d p2, Scene scene) {
        //  inside = empty set
        //  intersections = empty sorted set
        //  for each object in objects
        //    intersect ray with object, add to intersections
        //    if (ray origin is inside object) add object to inside set
        //  done
        //
        //  trans = 1
        //  for each intersection in intersections
        //    if (inside size != 0) calculate volume shader ...
        //
        //    if (intersection type is "IN") add object to inside set
        //    else remove object to inside set
        //
        //    trans *= intersection transparency
        //    if (trans = 0) break;
        //  done
        //
        // return trans
        
        Set<Object> inside = new HashSet<Object>();
        Set<Intersection> intersections = new TreeSet<Intersection>();
        Vector3d diff = new Vector3d(p2);
        diff.sub(r.getOrigin());
        double t = diff.length();

        for (Object o: scene.getObjects()) {
            for (Intersection i: o.intersect(r)) {
                if (i.getSortValue() < t - 0.00001) intersections.add(i);
            }
            
            if (o.inside(r.origin, r.getTime())) inside.add(o);
        }
        
        Color trans = new Color(1,1,1);        
        
        for (Intersection i: intersections) {
            ShadeContext shadeContext = new ShadeContext(((SimpleObject)i.getObject()).transformation, i, scene);
            shadeContext.setTime(r.getTime());
            
            if (inside.size() != 0) {
                // calc volume shader
            }
            
            trans.mul(i.getObject().getSurfaceShader().filter(r, shadeContext));
            
            if (i.getType() == Intersection.IN) {
                inside.add(i.getObject());
            } else if (i.getType() == Intersection.OUT) {
                inside.remove(i.getObject());
            }
            
            if (trans.value() < 0.001) break;
        }
        
        return Color.mul(trans, r.getColor());
    }
    
    public static Color trace(Ray r, Scene scene) {
        //  inside = empty set
        //  intersections = empty sorted set
        //  for each object in objects
        //    intersect ray with object, add to intersections
        //    if (ray origin is inside object) add object to inside set
        //  done
        //
        //  sum = 0, trans = 1
        //  for each intersection in intersections
        //    if (inside size != 0) calculate volume shader ...
        //
        //    for each lightsource in lightsources
        //      create lightray
        //      calculate shadow from lightray origin to intersection
        //      sum += shade intersection * trans
        //    done
        //
        //    if (intersection type is "IN") add object to inside set
        //    else remove object to inside set
        //
        //    trans *= intersection transparency
        //    if (trans = 0) break;
        //  done
        Set<Object> inside = new HashSet<Object>();
        Set<Intersection> intersections = new TreeSet<Intersection>();
        for (Object o: scene.getObjects()) {
            intersections.addAll(o.intersect(r));
            
            if (o.inside(r.origin, r.getTime())) inside.add(o);
        }
        
        Color ret = new Color(0,0,0), filter = new Color(1,1,1);
        
        for (Intersection i: intersections) {
            ShadeContext shadeContext = new ShadeContext(((SimpleObject)i.getObject()).transformation, i, scene);
            shadeContext.setTime(r.getTime());
            
            if (inside.size() != 0) {
                // calc volume shader
            }
            
            for (SurfaceModifier modifier: i.getObject().getSurfaceModifiers()) {
                modifier.modify(shadeContext);
            }

            ShadeReturn sret = i.getObject().getSurfaceShader().shade(r, shadeContext);
            ret.add(sret.getColor());
            filter.mul(sret.getFilter());
                        
            if (i.getType() == Intersection.IN) {
                inside.add(i.getObject());
            } else if (i.getType() == Intersection.OUT) {
                inside.remove(i.getObject());
            }
            /*
            ret.add(Color.mul(shadeContext.getColor(), trans));
            ret.add(Color.mul(shadeContext.getAmbient(), trans));
            
            trans.set(Color.mul(trans, shadeContext.getTransparency()));
            */
            
            if (filter.value() < 0.001) break;
        }
        
        return ret;
    }
    
    public static Image getImage(int width, int height, Scene scene, Camera camera) {
        double filmWidth = 0.036, filmHeight = ((double)height/width)*filmWidth;
        Image image = new Image(width, height);
        int nSamples = 16;
        
        // for 0 to height
        //   for 0 to width
        //     create ray
        //     trace ray
        //   done
        // done
        
        Color c[] = new Color[nSamples];
        double jitter = filmWidth/width;
        for (int y=0;y<height;y++) {
            double dy = filmHeight * (0.5 - (double)y/(height-1));
            
            for (int x=0;x<width;x++) {
                double dx = filmWidth * ((double)x/(width-1) - 0.5);
                
                for (int i=0;i<nSamples;i++) {
                    Ray r = camera.getRay(dx + Math.random()*jitter, dy - Math.random()*jitter, 0.0);
                    c[i] = trace(r, scene);
                }
                
                image.setPixel(x, y, c);
            }
        }
        
        return image;
    }
    
    public static void main(String args[]) throws Exception {
        Scene scene = new Scene();
        Camera camera = new Camera();
        camera.setTransformation(new Translation(0,0,5));
        camera.setAperture(1.4);
        camera.setFocalLength(50);
        camera.setFocalDistance(4.5);
        camera.setStochastic(true);
        
        Shape shape1 = new Sphere(new Point3d(-2, 0, -2), 1.0);
        Shape shape2 = new Sphere();
        Shape shape3 = new Sphere(new Point3d(2, 0, 2), 1.0);
        Shape shape4 = new Plane(new Point3d(0, -1, 0), new Vector3d(0,1,0));
        
        SurfaceShader shader1 = new PhongShader();
        SurfaceModifier modifier1 = new Bumpy();
        SurfaceModifier modifier2 = new ChromeRust();
        Object o1 = new SimpleObject(shape1);
        Object o2 = new SimpleObject(shape2);
        Object o3 = new SimpleObject(shape3);
        Object o4 = new SimpleObject(shape4);
        
        o1.setSurfaceShader(shader1);
        o2.setSurfaceShader(shader1);
        o2.addSurfaceModifier(modifier1);
        o2.addSurfaceModifier(modifier2);
        o3.setSurfaceShader(shader1);
        o4.setSurfaceShader(shader1);
        
        Lightsource light1 = new PointLightsource(new Point3d(2,2,2), Color.WHITE);
        Lightsource light2 = new PointLightsource(new Point3d(1,2,4), Color.WHITE);
        Lightsource light3 = new SphereLightsource(new Point3d(1,2,1), 0.25, new Color(0.55, 0.55, 0.5));
        AmbientLightsource light4 = new SimpleAmbientLightsource(new Color(0.2, 0.2, 0.25));
        AmbientLightsource light5 = new SampledAmbientLightsource(8, new Color(0.5, 0.5, 0.55));
        
        scene.addObject(o1);
        scene.addObject(o2);
        scene.addObject(o3);
        scene.addObject(o4);
        scene.addLightsource(light1);
        scene.addLightsource(light2);
        scene.addLightsource(light3);
        scene.addAmbientLightsource(light4);
        scene.addAmbientLightsource(light5);
        
        long t = System.currentTimeMillis();
        Image image = getImage(640, 480, scene, camera);
        System.out.println("time: " + (System.currentTimeMillis() - t));
        
        image.save(new File("/tmp/out-" + System.currentTimeMillis() + ".png"), "png");
        image.saveVariance(new File("/tmp/variance-" + System.currentTimeMillis() + ".png"), "png");
    }
}
