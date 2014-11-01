attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec4 a_color;
attribute vec2 a_texcoord;

uniform mat4 u_mvpmatrix;
uniform mat4 u_mvmatrix;
uniform vec3 u_lightpos;

varying vec4 v_color;
varying vec2 v_texcoord;

void main()
{
    //vec3 mvVertex = vec3(u_mvmatrix * a_position);
    //float distance = length(u_lightpos - mvVertex);

    //vec3 lightVector = normalize(u_lightpos - mvVertex);
    //vec3 mvNormal = vec3(u_mvmatrix * vec4(a_normal, 0.0));
    //float diffuse = max(dot(mvNormal, lightVector), 0.1);

    //diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));
    //v_color = a_color * diffuse;

    v_color = a_color;
    gl_Position = u_mvpmatrix * a_position;
    v_texcoord = a_texcoord;
}
