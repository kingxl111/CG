#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(const glm::vec3& o, const glm::vec3& d) : origin(o), direction(glm::normalize(d)) {}
};

struct Material {
    glm::vec3 ambient;  
    glm::vec3 diffuse;  
    glm::vec3 specular; 
    float shininess;    
    
    Material(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shin)
        : ambient(amb), diffuse(diff), specular(spec), shininess(shin) {}
};

struct Sphere {
    glm::vec3 center;
    float radius;
    Material material;

    Sphere(const glm::vec3& c, float r, const Material& m) : center(c), radius(r), material(m) {}
    
    bool intersect(const Ray& ray, float& t) const {
        glm::vec3 oc = ray.origin - center;
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(oc, ray.direction);
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4.0f * a * c;
        
        if (discriminant > 0) {
            t = (-b - sqrt(discriminant)) / (2.0f * a);
            if (t > 0.001f) return true;
        }
        return false;
    }
    
    glm::vec3 normal(const glm::vec3& point) const {
        return glm::normalize(point - center);
    }
};

struct Plane {
    glm::vec3 normal;
    float d; 
    Material material;
    
    Plane(const glm::vec3& n, float d, const Material& m) : normal(n), d(d), material(m) {}
    
    bool intersect(const Ray& ray, float& t) const {
        float denominator = glm::dot(normal, ray.direction);
        if (fabs(denominator) > 0.001f) {
            t = -(glm::dot(ray.origin, normal) + d) / denominator;
            return t >= 0.001f;
        }
        return false;
    }
};

glm::vec3 traceRay(const Ray& ray, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const glm::vec3& lightPos, const glm::vec3& cameraPos) {
    float t_min = 1e20;
    glm::vec3 hitColor = glm::vec3(0.0f);
    const Sphere* hitSphere = nullptr;
    const Plane* hitPlane = nullptr;

    for (const auto& sphere : spheres) {
        float t = 0.0f;
        if (sphere.intersect(ray, t) && t < t_min) {
            t_min = t;
            hitSphere = &sphere;
        }
    }

    for (const auto& plane : planes) {
        float t = 0.0f;
        if (plane.intersect(ray, t) && t < t_min) {
            t_min = t;
            hitPlane = &plane;
        }
    }

    if (hitSphere) {
        glm::vec3 hitPoint = ray.origin + ray.direction * t_min;
        glm::vec3 normal = hitSphere->normal(hitPoint);

        glm::vec3 ambient = hitSphere->material.ambient;
        
        glm::vec3 lightDir = glm::normalize(lightPos - hitPoint);
        float diff = std::max(glm::dot(normal, lightDir), 0.0f);
        glm::vec3 diffuse = hitSphere->material.diffuse * diff;
        
        glm::vec3 reflectDir = glm::reflect(-lightDir, normal);
        glm::vec3 viewDir = glm::normalize(cameraPos - hitPoint);
        float spec = pow(std::max(glm::dot(reflectDir, viewDir), 0.0f), hitSphere->material.shininess);
        glm::vec3 specular = hitSphere->material.specular * spec;
        
        hitColor = ambient + diffuse + specular;
    }
    else if (hitPlane) {
        glm::vec3 hitPoint = ray.origin + ray.direction * t_min;
        glm::vec3 normal = hitPlane->normal;
        
        glm::vec3 ambient = hitPlane->material.ambient;
        
        glm::vec3 lightDir = glm::normalize(lightPos - hitPoint);
        float diff = std::max(glm::dot(normal, lightDir), 0.0f);
        glm::vec3 diffuse = hitPlane->material.diffuse * diff;
        
        glm::vec3 reflectDir = glm::reflect(-lightDir, normal);
        glm::vec3 viewDir = glm::normalize(cameraPos - hitPoint);
        float spec = pow(std::max(glm::dot(reflectDir, viewDir), 0.0f), hitPlane->material.shininess);
        glm::vec3 specular = hitPlane->material.specular * spec;
        
        hitColor = ambient + diffuse + specular;
    }

    return hitColor;
}

int main() {
    glm::vec3 cameraPos(0.0f, 0.0f, 5.0f); 
    glm::vec3 lightPos(2.0f, 5.0f, -2.0f); 

    Material sphereMaterial(glm::vec3(0.2f), glm::vec3(0.7f, 0.3f, 0.3f), glm::vec3(1.0f), 32.0f);
    Material planeMaterial(glm::vec3(0.2f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(0.8f), 16.0f);

    std::vector<Sphere> spheres = { Sphere(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, sphereMaterial) };
    std::vector<Plane> planes = { Plane(glm::vec3(0.0f, 1.0f, 0.0f), -1.0f, planeMaterial) };

    sf::Window window(sf::VideoMode(800, 600), "Ray Tracing - Phong Shading");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glPointSize(1.0f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int y = 0; y < 600; ++y) {
            for (int x = 0; x < 800; ++x) {
                float u = (2.0f * x - 800.0f) / 800.0f;
                float v = (2.0f * y - 600.0f) / 600.0f;
                Ray ray(cameraPos, glm::normalize(glm::vec3(u, v, -1.0f)));

                glm::vec3 color = traceRay(ray, spheres, planes, lightPos, cameraPos);

                glBegin(GL_POINTS);
                glColor3f(color.r, color.g, color.b);
                glVertex2f(x / 800.0f * 2.0f - 1.0f, y / 600.0f * 2.0f - 1.0f);
                glEnd();
            }
        }

        window.display();
    }

    return 0;
}
