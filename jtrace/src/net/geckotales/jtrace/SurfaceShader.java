package net.geckotales.jtrace;

public abstract class SurfaceShader {
    public ShadeReturn shade(Ray e, ShadeContext shadeContext) {
        ShadeReturn ret = new ShadeReturn();
        
        if (usesAmbientLight()) {
            for (AmbientLightsource light: shadeContext.getScene().getAmbientLightsources()) {
                ret.getColor().add(shadeAmbient(light.ambient(shadeContext), shadeContext));
            }
        }
        
        if (usesDirectionalLight()) {
            for (Lightsource light: shadeContext.getScene().getLightsources()) {
                Ray r = light.createRay(shadeContext);                
                r.setColor(JTrace.shadow(r, shadeContext.getGlobalPosition(), shadeContext.getScene()));
                ShadeReturn ret2 = shadeDirectional(e, r, shadeContext);
                ret.getColor().add(ret2.getColor());
            }
        }
        
       ret.getTransparency().mul(transparency(e, shadeContext));
       ret.getFilter().mul(filter(e, shadeContext));
        
        return ret;
    }

    abstract public ShadeReturn shadeDirectional(Ray e, Ray r, ShadeContext shadeContext);
    abstract public Color shadeAmbient(Color ambient, ShadeContext shadeContext);
    
    public Color transparency(Ray e, ShadeContext shadeContext) {
        return Color.BLACK;
    }
    
    public Color filter(Ray e, ShadeContext shadeContext) {
        return Color.BLACK;
    }    
    
    public boolean usesAmbientLight() {
        return true;
    }
    
    public boolean usesDirectionalLight() {
        return true;
    }
    
}