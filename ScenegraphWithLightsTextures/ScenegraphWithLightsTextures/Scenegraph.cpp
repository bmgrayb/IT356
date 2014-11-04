#include "Scenegraph.h"
#include <stack>
#include "TransformNode.h"
using namespace std;
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <fstream>
#include <sstream>
Scenegraph::Scenegraph()
{
    root = NULL;
}

void Scenegraph::makeScenegraph(Node *r)
{
    if (root!=NULL)
    {
        delete root;
        root = NULL;
    }
    this->root = r;
}



Scenegraph::~Scenegraph()
{
    if (root!=NULL)
    {
        delete root;
        root = NULL;
    }


}

void Scenegraph::initShaderProgram(GLint shaderProgram)
{
	program = shaderProgram;
  // modelviewLocation = glGetUniformLocation(shaderProgram,"modelview");
 //objectColorLocation = glGetUniformLocation(shaderProgram,"vColor");
	 projectionLocation = glGetUniformLocation(shaderProgram,"projection");
    modelviewLocation = glGetUniformLocation(shaderProgram,"modelview");
    normalMatrixLocation = glGetUniformLocation(shaderProgram,"normalmatrix");
    numLightsLocation = glGetUniformLocation(shaderProgram,"numLights");


    mat_ambientLocation = glGetUniformLocation(shaderProgram,"material.ambient");
    mat_diffuseLocation = glGetUniformLocation(shaderProgram,"material.diffuse");
    mat_specularLocation = glGetUniformLocation(shaderProgram,"material.specular");
    mat_shininessLocation = glGetUniformLocation(shaderProgram,"material.shininess");
	


}

void Scenegraph::draw(stack<glm::mat4>& modelView)
{
    if (root!=NULL)
    {
		lights.clear();
		lightlocation.clear();
		root->returnLights(lights,modelView);
	lightlocation.resize(lights.size());
	for( int i=0; i<lightlocation.size();i++){
	stringstream name;

        name << "light[" << i << "].ambient";

        lightlocation[i].ambientLocation = glGetUniformLocation(program,name.str().c_str());

        name.clear();//clear any bits set
        name.str(std::string());

        name << "light[" << i << "].diffuse";
	//	cout<<name.str().c_str();
        lightlocation[i].diffuseLocation = glGetUniformLocation(program,name.str().c_str());

        name.clear();//clear any bits set
        name.str(std::string());

        name << "light[" << i << "].specular";

        lightlocation[i].specularLocation = glGetUniformLocation(program,name.str().c_str());

        name.clear();//clear any bits set
        name.str(std::string());

        name << "light[" << i << "].position";

        lightlocation[i].positionLocation = glGetUniformLocation(program,name.str().c_str());

        name.clear();//clear any bits set
        name.str(std::string());
	}

	glUniform1i(numLightsLocation,lightlocation.size());

    //light properties
    for (int i=0;i<lights.size();i++)
    {
		//cout<<lights[i].getDiffuse().x;
        glUniform3fv(lightlocation[i].ambientLocation,1,glm::value_ptr(lights[i].getAmbient()));
        glUniform3fv(lightlocation[i].diffuseLocation,1,glm::value_ptr(lights[i].getDiffuse()));
        glUniform3fv(lightlocation[i].specularLocation,1,glm::value_ptr(lights[i].getSpecular()));
        glUniform4fv(lightlocation[i].positionLocation,1,glm::value_ptr(lights[i].getPosition()));
    }

		
        root->draw(modelView);
    }

	if (root!=NULL)
	{
		root->updateBB();
		root->drawBB(modelView);
	}
}

void Scenegraph::animate(float time)
{
}


