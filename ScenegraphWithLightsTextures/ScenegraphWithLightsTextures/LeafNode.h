#ifndef _LEAF_H_
#define _LEAF_H_
#include "node.h"
#include "Object.h"
#include "Material.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.h"


class LeafNode : public Node
{
public:

	LeafNode(Object *instanceOf,Scenegraph *graph,string name="")
		:Node(graph,name)
	{
		this->instanceOf = instanceOf;
		//default material
		material.setAmbient(1.0f,0.6f,0.6f);
		material.setDiffuse(1.0f,0.6f,0.6f);
		material.setSpecular(0.2f,0.1f,0.1f);
		material.setShininess(1);
	}

	~LeafNode(void)
	{
	}

	Node *clone()
	{
		LeafNode *newclone = new LeafNode(instanceOf,scenegraph,name);
		newclone->setMaterial(material);

		return newclone;
	}

	virtual void draw(stack<glm::mat4> &modelView)
    {
		GLuint a;
        if (instanceOf!=NULL)
		{
			 //get the color
           
            //set the color for all vertices to be drawn for this object
     //       glUniform3fv(scenegraph->objectColorLocation,1,glm::value_ptr(color));
			a = glGetError();
			glUniformMatrix4fv(scenegraph->modelviewLocation,1,GL_FALSE,glm::value_ptr(modelView.top()));
			glUniform3fv(scenegraph->mat_ambientLocation,1,glm::value_ptr(material.getAmbient()));
			glUniform3fv(scenegraph->mat_diffuseLocation,1,glm::value_ptr(material.getDiffuse()));
			glUniform3fv(scenegraph->mat_specularLocation,1,glm::value_ptr(material.getSpecular()));
			glUniform1i(scenegraph->mat_shininessLocation,material.getShininess());
			a = glGetError();
			instanceOf->draw();        
			a = glGetError();
		}
    }

	virtual void drawBB(stack<glm::mat4>& modelView)
	{
		if (bbDraw)
		{
			GLuint a;
			glm::mat4 bbTransform;

			bbTransform = glm::translate(glm::mat4(1.0),0.5f*(minBounds+maxBounds)) * glm::scale(glm::mat4(1.0),maxBounds-minBounds);
			glm::vec4 color = glm::vec4(1,1,1,1);
			//set the color for all vertices to be drawn for this object
			glUniform3fv(scenegraph->objectColorLocation,1,glm::value_ptr(color));
			glUniformMatrix4fv(scenegraph->modelviewLocation,1,GL_FALSE,glm::value_ptr(modelView.top() * bbTransform));
			a = glGetError();
			scenegraph->getInstance("box")->draw();        		
		}
	}
	
	virtual void updateBB()
	{
		minBounds = instanceOf->getMinimumWorldBounds().xyz();
		maxBounds = instanceOf->getMaximumWorldBounds().xyz();
	}

	glm::vec4 getColor()
	{
		return material.getAmbient();
	}

	/*
		*Set the material of each vertex in this object
		*/
	virtual void setMaterial(Material& mat)
	{
		material = mat;
	}

	/*
		* gets the material
		*/
	Material getMaterial()
	{
		return material;
	}

	void setTexture(Texture *tex)
	{
		cout << "Texture set to " << tex->getName() << endl;
	}
	void addLight(const Light &l)
	{
		//cout << "Light added in node " << name << endl;
		
		 lights.push_back(l);
		
	}
	virtual void returnLights(vector<Light>& vLights, stack<glm::mat4>& modelView){
		for(Light l: lights){
			l.setPosition(modelView.top() * l.getPosition());
			 vLights.push_back(l);
		}
	}

protected:
	Object *instanceOf;
	Material material;
};
#endif
