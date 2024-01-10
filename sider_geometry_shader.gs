#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

in VS_OUT {
    vec2 texCoords;
} gs_in[];

// ���ݸ�Ƭ����ɫ�������
out vec3 fragPos;
out vec3 normal;
out vec2 TexCoords; 

uniform float time;
uniform bool is_collide;
uniform float collision_time;

vec3 physical_simulation(float time, vec3 normal) 
{
    float gravity = 9.8; // �������ٶȣ���λʱ�����ٶȵ���������
    float discount_gravity = 0.15; // ��������Ӱ��
    float initial_velocity = 1.5; // ���ٶ�
    float resistance_factor = 0.3; // ��������ϵ��

    // Calculate the displacement along the normal due to initial velocity and resistance
    vec3 normal_displacement = (initial_velocity * time - 0.5 * resistance_factor * time * time) * normal; // Corrected 0.05 to 0.5 to be consistent with gravity displacement calculation

    // �������������������λ�ƣ�������ٶ�Ϊ0����λ��Ϊ1/2 * g * t^2
    float gravity_displacement = 0.5 * discount_gravity * gravity * time * time;

    vec3 direction = normal_displacement;
    direction.y -= gravity_displacement;

    return direction;
}

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    if (is_collide){
        return position + vec4(physical_simulation(time - collision_time, normal), 0.0);
    }
    return position;
}


vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}


void EmitQuad(vec4 p0, vec4 p1, vec4 p2, vec4 p3, vec2 tc0, vec2 tc1, vec2 tc2)
{
    TexCoords = tc0;
    gl_Position = p0;
    EmitVertex();

    TexCoords = tc1;
    gl_Position = p1;
    EmitVertex();

    TexCoords = tc0;
    gl_Position = p2;
    EmitVertex();

    TexCoords = tc2;
    gl_Position = p3;
    EmitVertex();

    EndPrimitive();
}

void main() {
    normal = GetNormal();
    float thickness = 0.01;
    vec4 offset = vec4(normal * thickness, 0.0f); // thickness * normal

    // ��һ���ı���
    EmitQuad(explode(gl_in[0].gl_Position, normal) - offset,
             explode(gl_in[1].gl_Position, normal) - offset,
             explode(gl_in[0].gl_Position, normal),
             explode(gl_in[1].gl_Position, normal),
             gs_in[0].texCoords, gs_in[1].texCoords, gs_in[1].texCoords);
    fragPos = vec3(explode(gl_in[0].gl_Position, normal));

    // �ڶ����ı���
    EmitQuad(explode(gl_in[1].gl_Position, normal) - offset,
             explode(gl_in[2].gl_Position, normal) - offset,
             explode(gl_in[1].gl_Position, normal),
             explode(gl_in[2].gl_Position, normal),
             gs_in[1].texCoords, gs_in[2].texCoords, gs_in[2].texCoords);

    // �������ı���
    EmitQuad(explode(gl_in[2].gl_Position, normal) - offset,
             explode(gl_in[0].gl_Position, normal) - offset,
             explode(gl_in[2].gl_Position, normal),
             explode(gl_in[0].gl_Position, normal),
             gs_in[2].texCoords, gs_in[0].texCoords, gs_in[0].texCoords);
}
