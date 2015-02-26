#version 120

varying vec4 vColor;
varying vec3 vNormal;
varying vec3 vPosition;

vec4 lightSource( vec3 normal, vec3 position, gl_LightSourceParameters light )
{
        vec3 direction = normalize(light.position.xyz - position);
        vec3 eye = normalize(-position);
        vec3 reflect = normalize(-reflect(direction, normal));

        float NdotL = max(0.0, dot(normal, direction));
        float NdotH = max(0.0, dot(reflect, eye));

        float Idiff = NdotL;
        float Ispec = pow( NdotH, 0.3*gl_FrontMaterial.shininess );

        // 'real' shading
        return
                gl_FrontMaterial.ambient  * light.ambient +
                gl_FrontMaterial.diffuse  * light.diffuse  * Idiff +
                gl_FrontMaterial.specular * light.specular * Ispec;
}

vec4 lighting( void )
{
    // normal might be damaged by linear interpolation.
    vec3 normal = normalize( vNormal );

    return
        gl_FrontMaterial.emission +
        gl_FrontMaterial.ambient * gl_LightModel.ambient +
        lightSource( normal, vPosition, gl_LightSource[0] );
}

void main()
{
    gl_FragColor = vColor * lighting();
}
