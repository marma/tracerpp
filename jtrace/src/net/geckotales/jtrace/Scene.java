package net.geckotales.jtrace;

import java.io.*;
import java.util.*;

public class Scene {
    Vector<Lightsource> lightsources = new Vector<Lightsource>();
    Vector<AmbientLightsource> ambientLightsources = new Vector<AmbientLightsource>();
    Vector<Object> objects = new Vector<Object>();

    public void load(InputStream in) {        
    }
    
    public void addObject(Object o) {
        objects.add(o);
    }
    
    public void addLightsource(Lightsource l) {
        lightsources.add(l);
    }
    
    public void addAmbientLightsource(AmbientLightsource l) {
        ambientLightsources.add(l);
    }
    
    public Vector<Object> getObjects() {
        return objects;
    }
    
    public Vector<Lightsource> getLightsources() {
        return lightsources;
    }
    
    public Vector<AmbientLightsource> getAmbientLightsources() {
        return ambientLightsources;
    }
}