package net.geckotales.jtrace.lightsources;

import net.geckotales.jtrace.*;
import javax.vecmath.*;

public class SimpleAmbientLightsource implements AmbientLightsource {
    Color c;
    
    public SimpleAmbientLightsource(Color c) {
        this.c = c;
    }
    
    public Color ambient(ShadeContext shadeContext) {
        return c;
    }
}