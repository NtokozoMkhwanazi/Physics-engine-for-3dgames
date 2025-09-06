
class Model{

	public  : 
		 Model(char* path){
		 
		 	loadModel(path);
		 }
	
		void draw(Shader &shader);
		
	private :
	
		//model data
		vector<Mesh> meshes;
		string directory;
		
		void loadModel(string path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		vector<Texture> loadMaterialTextures(aiMaterial *mat,
		aiTextureType type, string typeName);

};
