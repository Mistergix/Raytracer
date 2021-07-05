#shader vertex
#version 330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 u_TranslationMatrix;
uniform mat4 u_RotationMatrix;
uniform mat4 u_ScaleMatrix;

uniform mat4 u_ProjectionMatrix;

uniform vec3 u_CameraPosition;
uniform vec3 u_TargetPosition;

varying vec3 v_VertPos;
varying vec2 v_UV;
varying vec3 v_Normal;
varying vec3 v_CamPos;

varying mat4 v_ModelMatrix;

mat4 LookAtRH(vec3 camPos, vec3 target, vec3 up)
{
    vec3 zaxis = normalize(camPos - target);    // The "forward" vector.
    vec3 xaxis = normalize(cross(up, zaxis));// The "right" vector.
    vec3 yaxis = normalize(cross(zaxis, xaxis));     // The "up" vector.

    // Create a 4x4 orientation matrix from the right, up, and forward vectors
    // This is transposed which is equivalent to performing an inverse 
    // if the matrix is orthonormalized (in this case, it is).
    mat4 orientation = {
       vec4(xaxis.x, yaxis.x, zaxis.x, 0),
       vec4(xaxis.y, yaxis.y, zaxis.y, 0),
       vec4(xaxis.z, yaxis.z, zaxis.z, 0),
       vec4(0,       0,       0,     1)
    };

    // Create a 4x4 translation matrix.
    // The eye position is negated which is equivalent
    // to the inverse of the translation matrix. 
    // T(v)^-1 == T(-v)
    mat4 translation = {
        vec4(1,      0,      0,   0),
        vec4(0,      1,      0,   0),
        vec4(0,      0,      1,   0),
        vec4(-camPos.x, -camPos.y, -camPos.z, 1)
    };

    // Combine the orientation and translation to compute 
    // the final view matrix. Note that the order of 
    // multiplication is reversed because the matrices
    // are already inverted.
    return (orientation * translation);
}

void main(void) {
    // FRAGMENT DATA
    v_VertPos = vertPos;
    v_UV = uv;
    v_Normal = normal;
	v_ModelMatrix = u_TranslationMatrix * u_RotationMatrix * u_ScaleMatrix;
    v_CamPos = u_CameraPosition;

	// mat4 camModelMat = u_CameraTranslationMatrix * u_CameraRotationMatrix;

	// mat4 viewMatrix = inverse(camModelMat);

    mat4 viewMatrix = LookAtRH(u_CameraPosition, u_TargetPosition, vec3(0.f, 1.f, 0.f));

	vec4 worldPosition = v_ModelMatrix * vec4(vertPos, 1.0);
	vec4 viewPosition = viewMatrix * worldPosition;
	vec4 finalPosition = u_ProjectionMatrix * viewPosition;

	gl_Position = finalPosition;
}



#shader fragment
#version 330 core

uniform sampler2D u_Texture;
uniform float u_Shininess;
uniform vec3 u_SpecularColor;

varying vec3 v_VertPos;
varying vec2 v_UV;
varying vec3 v_Normal;
varying vec3 v_CamPos;

varying mat4 v_ModelMatrix;

uniform struct Light {
    vec3 position;
    vec3 intensities; //a.k.a the color of the light
} light;

vec4 diffuse(vec3 N, vec3 L) {
    //calculate the cosine of the angle of incidence
    float brightness = max(0.0, dot(N, L));

    //calculate final color of the pixel, based on:
    // 1. The angle of incidence: brightness
    // 2. The color/intensities of the light: light.intensities
    // 3. The texture and texture coord: texture(tex, fragTexCoord)
    vec4 surfaceColor = texture(u_Texture, v_UV);

    return vec4(brightness * light.intensities * surfaceColor.rgb, surfaceColor.a);
}

vec4 specular(vec3 N, vec3 L, vec3 V) {
    
    vec3 incidenceVector = -L;
    vec3 reflectionVector = reflect(incidenceVector, N);

    float cosAngle = max(0.0, dot(V, reflectionVector));
    float specularCoefficient = pow(cosAngle, u_Shininess);

    vec3 specularComponent = specularCoefficient * u_SpecularColor * light.intensities;

    return vec4(specularComponent, 0.0);
}

void main(void)
{
    //calculate normal in world coordinates
    mat3 normalMatrix = transpose(inverse(mat3(v_ModelMatrix)));
    vec3 normal = normalize(normalMatrix * v_Normal);

    //calculate the location of this fragment (pixel) in world coordinates
    vec3 surfacePos = vec3(v_ModelMatrix * vec4(v_VertPos, 1.0));

    //calculate the vector from this pixels surface to the light source
    vec3 surfaceToLight = normalize(light.position - surfacePos);

    vec3 surfaceToCamera = normalize(v_CamPos - surfacePos);


    vec4 dColor = diffuse(normal, surfaceToLight);
    vec4 sColor = specular(normal, surfaceToLight, surfaceToCamera);

    gl_FragColor = dColor + sColor;
}