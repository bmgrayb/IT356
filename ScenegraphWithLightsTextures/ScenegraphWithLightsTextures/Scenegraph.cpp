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
		for(Light l: lights){
			//l.setPosition(modelView.top() * l.getPosition());
		}
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
		cout << "light location x" << lights[i].getPosition().x << endl;
		cout << "light location y" << lights[i].getPosition().y << endl;
		cout << "light location z" << lights[i].getPosition().z << endl;
		
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
	
	TransformNode * rideTransform = (TransformNode *) root->getNode("car-transform");
	TransformNode * seatTransform = (TransformNode *) root->getNode("seat-transform");

	if(rideTransform!=NULL)
		rideTransform->setAnimationTransform(glm::translate(glm::mat4(1.0),glm::vec3(sin(time*10)*100,0,0)) );//* glm::rotate(glm::mat4(1.0),time*10,glm::vec3(0,1,0)));
	
	if(seatTransform!=NULL)
		seatTransform->setAnimationTransform(glm::rotate(glm::mat4(1.0),time*10,glm::vec3(0,1,0)));

	if(root->getNode("chair") != NULL){
	TransformNode *ch1 = NULL;
	TransformNode *ch2 = NULL;
	TransformNode *ch3 = NULL;
	TransformNode *ch4 = NULL;
	TransformNode *ride = NULL;
	glm::mat4 temp = glm::mat4(1.0);
	glm::mat4 temp2 = glm::mat4(1.0);
	glm::mat4 temp3 = glm::mat4(1.0);
	glm::mat4 temp4 = glm::mat4(1.0);
	glm::mat4 temp5 = glm::mat4(1.0);

	temp = glm::translate(glm::mat4(1),glm::vec3(0,abs(cos(time*3))*12,0));
	temp2 = glm::translate(glm::mat4(1),glm::vec3(0,0,abs(cos(time*3))*12));
	temp3 = glm::translate(glm::mat4(1),glm::vec3(0,0,-1*abs(cos(time*3))*12));
	temp4 = glm::translate(glm::mat4(1),glm::vec3(0,-1*abs(cos(time*3))*12,0));
	//temp5 *= glm::rotate(glm::mat4(1.0),glm::radians(time*50),glm::vec3(0,1,0));
	//temp5 *= glm::rotate(glm::mat4(1.0),glm::radians(sin(time*5.0f)*30),glm::vec3(0,0,1));
	//temp5 *= glm::rotate(glm::mat4(1.0),glm::radians(cos(time*5.0f)*30),glm::vec3(1,0,0));
	//temp5 *= glm::rotate(glm::mat4(1.0),glm::radians(40.0f),glm::vec3(1,0,0));
	
	temp *= glm::rotate(glm::mat4(1.0),glm::radians(time*500),glm::vec3(1,0,0));
	temp2 *= glm::rotate(glm::mat4(1.0),glm::radians(time*500),glm::vec3(1,0,0));
	temp3 *= glm::rotate(glm::mat4(1.0),glm::radians(time*500),glm::vec3(1,0,0));
	temp4 *= glm::rotate(glm::mat4(1.0),glm::radians(time*500),glm::vec3(1,0,0));

	ch1 = static_cast<TransformNode*> (root->getNode("chair"));
	ch2 = static_cast<TransformNode*>(root->getNode("chair2"));
	ch3 = static_cast<TransformNode*>(root->getNode("chair3"));
	ch4 =static_cast<TransformNode*>( root->getNode("chair4"));
	ride = static_cast<TransformNode*>( root->getNode("ride"));
	ch1->setAnimationTransform(temp);
	ch2->setAnimationTransform(temp2);
	ch3->setAnimationTransform(temp3);
	ch4->setAnimationTransform(temp4);
	ride->setAnimationTransform(temp5);
	

	}

}


