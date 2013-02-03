package net.geckotales.jtrace;

import javax.vecmath.*;

public interface Lightsource {
    public Ray createRay(ShadeContext shadeContext);
    public boolean isDistributed();
}