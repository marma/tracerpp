package net.geckotales.jtrace;

import javax.vecmath.*;

public interface AmbientLightsource {
    public Color ambient(ShadeContext shadeContext);
}