#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_TranslationMatrix;
uniform mat4 u_RotationXMatrix;
uniform mat4 u_RotationYMatrix;
uniform mat4 u_RotationZMatrix;
uniform mat4 u_ScaleMatrix;

uniform mat4 u_ProjectionMatrix;

uniform vec3 u_CameraPosition;
uniform vec3 u_TargetPosition;

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

void main()
{
    mat4 rotationMatrixXYZ = u_RotationZMatrix * u_RotationYMatrix * u_RotationXMatrix;
    mat4 modelMatrix = u_TranslationMatrix * rotationMatrixXYZ * u_ScaleMatrix;
    mat4 viewMatrix = LookAtRH(u_CameraPosition, u_TargetPosition, vec3(0.f, 1.f, 0.f));

    vec4 worldPosition = modelMatrix * vec4(position, 1.0);
    vec4 viewPosition = viewMatrix * worldPosition;
    vec4 finalPosition = u_ProjectionMatrix * viewPosition;

    gl_Position = finalPosition;
};

#shader fragment
#version 330 core

out vec4 color;

void main()
{
	color = vec4(1.0);
};