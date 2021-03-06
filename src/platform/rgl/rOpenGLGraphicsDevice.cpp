#include "rgl/rOpenGLGraphicsDevice.hpp"

rOpenGLGraphicsDevice::rOpenGLGraphicsDevice(){
	m_isInit = false;
	m_nextRenderbufferId = 0;
	m_defaultFramebuffer = 0;
	m_activeRenderBufferId = 0;

	m_clearColor.Set(0, 0, 0, 255);
}

bool rOpenGLGraphicsDevice::IsInit() const {
	return m_isInit;
}

bool rOpenGLGraphicsDevice::Init(){
	rColor c(0, 0, 0, 255);
	SetClearColor(c);
	
	glEnable(GL_DEPTH_TEST);

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_defaultFramebuffer);

	m_isInit = true;
    return m_isInit;
}

void rOpenGLGraphicsDevice::DeleteShaderProgram(unsigned int shaderId){
	glDeleteProgram(shaderId);
}

void rOpenGLGraphicsDevice::Clear(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void rOpenGLGraphicsDevice::EnableDepthTesting(bool enable){
	if (enable){
		glEnable(GL_DEPTH_TEST);
	}
	else{
		glDisable(GL_DEPTH_TEST);
	}
}

void rOpenGLGraphicsDevice::EnablePolygonFillOffset(bool enable){
	if (enable){
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);
	}
	else{
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.0f, 1.0f);
	}
}

void rOpenGLGraphicsDevice::SetClearColor(const rColor& color){
	m_clearColor = color;

	glClearColor(float(color.red) / 255.0f, float(color.green) / 255.0f, float(color.blue) / 255.0f, float(color.alpha) / 255.0f);
}

unsigned int rOpenGLGraphicsDevice::CreateShaderProgram(const rString& vertex, const rString& fragment){
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;


    vertexShader = CompileShader(GL_VERTEX_SHADER, vertex.c_str());

    if (!vertexShader)
	    return 0;

    fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragment.c_str());

    if (!fragmentShader){
	    glDeleteShader(vertexShader);
	    return 0;
    }

    programObject = glCreateProgram();

    if (!programObject)
	    return 0;

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    glLinkProgram(programObject);
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    if (!linked){
	GLint infoLen = 0;
	glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

	if (infoLen > 1){
	     char* infoLog = new char[infoLen];

	     glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
	     m_lastError.assign(infoLog, infoLen);         

	     delete [] infoLog;
	}

	glDeleteProgram(programObject);
	return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    m_lastError.clear();
    return programObject;
}

GLuint rOpenGLGraphicsDevice::CompileShader(GLenum type, const char* program){
    GLuint shader;
    GLint compiled;

    shader = glCreateShader(type);

    if (!shader)
	    return 0;

    glShaderSource(shader, 1, &program, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	
    if (!compiled){
      GLint infoLen = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

      if (infoLen > 1){
	 char* infoLog = new char[infoLen];

	 glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
	 m_lastError.assign(infoLog, infoLen);
	 rLog::Error(m_lastError);

	 delete [] infoLog;
      }

      glDeleteShader(shader);
      return 0;
    }

    m_lastError.clear();
    return shader;
}

void rOpenGLGraphicsDevice::Uninit(){
	m_isInit = false;
}

void rOpenGLGraphicsDevice::SetViewport(int x , int y, int width, int height) {
	glViewport(x, y, width, height);
}

void rOpenGLGraphicsDevice::SetActiveMaterial(rMaterial* material){
	const rPropertyCollection& materialParameters = material->Parameters();
	int textureIndex = 0;
	GLint programId = material->Shader()->ProgramId();
	rArrayString paramNames;
	material->GetParameterNames(paramNames);
	
	glUseProgram(programId);
	
	for (size_t i = 0; i < paramNames.size(); i++){
		rString paramName = paramNames[i];
		rPropertyType paramType = materialParameters.GetType(paramName);
		GLint uniformHandle = glGetUniformLocation ( programId, paramNames[i].c_str() );
		
		switch (paramType){
			case rPROPERTY_TYPE_COLOR:{
				rColor color;
				materialParameters.GetColor(paramName, color);
				glUniform4f(uniformHandle, color.red / 255.0f, color.green / 255.0f, color.blue / 255.0f , color.alpha / 255.0f);
			}
			break;

			case rPROPERTY_TYPE_TEXTURE:{
				rTexture2D* texture = NULL;
				materialParameters.GetTexture(paramName, texture);
				glActiveTexture ( GL_TEXTURE0 + textureIndex);
				glBindTexture ( GL_TEXTURE_2D, texture->GraphicsDeviceID() );
				glUniform1i ( uniformHandle, textureIndex );
				textureIndex ++;

				if (texture->Bpp() > 3){
					glEnable (GL_BLEND);
					glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				}
				else{
					glDisable(GL_BLEND);
				}

			}
			break;

			case rPROPERTY_TYPE_FLOAT:{
				float f = 0.0f;
				materialParameters.GetFloat(paramName, f);
				glUniform1f(uniformHandle, f);
			}
			break;
		}


	}
}

unsigned int rOpenGLGraphicsDevice::CreateArrayBuffer(const char* data, size_t dataSize){
	GLuint bufferId;
	glGenBuffers(1, &bufferId);
	
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	
	return bufferId;
}

unsigned int rOpenGLGraphicsDevice::CreateElementBuffer(const unsigned short* elementData, size_t elementDataSize){
	GLuint bufferId;
	glGenBuffers(1, &bufferId);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementDataSize, elementData, GL_STATIC_DRAW);
	
	return bufferId;
}

void rOpenGLGraphicsDevice::DeleteBuffer(unsigned int bufferId){
	glDeleteBuffers(1, &bufferId);
}

unsigned int rOpenGLGraphicsDevice::CreateTexture(int width, int height, int bpp , const unsigned char* data){
	unsigned char* oglTexData = ReflectTexture(width, height, bpp, data);

	GLenum format = (bpp == 3) ? GL_RGB : GL_RGBA;
	GLuint textureID;
	
	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, oglTexData);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	delete[] oglTexData;

	return textureID;
}

unsigned char* rOpenGLGraphicsDevice::ReflectTexture(int width, int height, int bpp , const unsigned char* data){
	unsigned char* oglData = new unsigned char[width * height * bpp];
	unsigned int rowSize = width * bpp;

	int srcRow = height - 1;
	int destRow = 0;

	while (srcRow >= 0){
		memcpy(oglData + (destRow * rowSize), data + (srcRow * rowSize), rowSize);

		srcRow--;
		destRow++;
	}

	return oglData;
}

void rOpenGLGraphicsDevice::UnregisterTexture(int textureID){
	GLuint texID(textureID);
	glDeleteTextures(1, &texID);
}

void rOpenGLGraphicsDevice::RenderGeometry(const rGeometry* geometry, const rMatrix4& transform, const rString& elementBufferName, rMaterial* material){
	if (geometry && material){
		rElementBuffer* elementBuffer = geometry->GetElementBuffer(elementBufferName);
	
		if (elementBuffer){
			SetActiveMaterial(material);
		
			switch (geometry->GeometryProfile()){
				case rGeometryProfile::TEXCOORD:
					RenderTexCoordGeometryProfile(geometry, transform, elementBuffer, material);
					break;

				case rGeometryProfile::VERTEXCOLOR:
					RenderVertexColorGeometryProfile(geometry, transform, elementBuffer, material);
					break;
			}
		}
	}
}

void rOpenGLGraphicsDevice::RenderTexCoordGeometryProfile(const rGeometry* geometry, const rMatrix4& transform, rElementBuffer* elementBuffer, rMaterial* material){
	GLint programId = material->Shader()->ProgramId();
	GLuint vertexBufferId = geometry->VertexBufferId();
	GLuint elementBufferId = elementBuffer->BufferId();

	GLuint gPositionLoc = glGetAttribLocation(programId, "recPosition");
	GLuint gNormalLoc = glGetAttribLocation(programId, "recNormal");
	GLuint gTexCoordLoc = glGetAttribLocation(programId, "recTexCoord");

	GLuint gMatrixLoc = glGetUniformLocation(programId, "recMVPMatrix");

	glUniformMatrix4fv(gMatrixLoc, 1, GL_FALSE, transform.m);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glVertexAttribPointer(gPositionLoc, 3, GL_FLOAT, GL_FALSE, 32, 0);
	glEnableVertexAttribArray(gPositionLoc);

	glVertexAttribPointer(gNormalLoc, 3, GL_FLOAT, GL_FALSE, 32, (void*)12);
	glEnableVertexAttribArray(gNormalLoc);

	glVertexAttribPointer(gTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 32, (void*)24);
	glEnableVertexAttribArray(gTexCoordLoc);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
	glDrawElements(GLGeometryType(elementBuffer->GeometryType()), elementBuffer->Size(), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(gPositionLoc);
	glDisableVertexAttribArray(gTexCoordLoc);
	glDisableVertexAttribArray(gNormalLoc);
}

void rOpenGLGraphicsDevice::RenderVertexColorGeometryProfile(const rGeometry* geometry, const rMatrix4& transform, rElementBuffer* elementBuffer, rMaterial* material){
	GLint programId = material->Shader()->ProgramId();
	GLuint vertexBufferId = geometry->VertexBufferId();
	GLuint elementBufferId = elementBuffer->BufferId();

	GLuint gPositionLoc = glGetAttribLocation(programId, "recPosition");
	GLuint gNormalLoc = glGetAttribLocation(programId, "recNormal");
	GLuint gVertexColorLoc = glGetAttribLocation(programId, "recVertexColor");

	GLuint gMatrixLoc = glGetUniformLocation(programId, "recMVPMatrix");

	glUniformMatrix4fv(gMatrixLoc, 1, GL_FALSE, transform.m);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glVertexAttribPointer(gPositionLoc, 3, GL_FLOAT, GL_FALSE, 40, 0);
	glEnableVertexAttribArray(gPositionLoc);

	glVertexAttribPointer(gNormalLoc, 3, GL_FLOAT, GL_FALSE, 40, (void*)12);
	glEnableVertexAttribArray(gNormalLoc);

	glVertexAttribPointer(gVertexColorLoc, 4, GL_FLOAT, GL_FALSE, 40, (void*)24);
	glEnableVertexAttribArray(gVertexColorLoc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
	glDrawElements(GLGeometryType(elementBuffer->GeometryType()), elementBuffer->Size(), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(gPositionLoc);
	glDisableVertexAttribArray(gVertexColorLoc);
	glDisableVertexAttribArray(gNormalLoc);
}

void rOpenGLGraphicsDevice::RenderImmediate(const rImmediateBuffer& geometry, const rMatrix4& transform, rMaterial* material){
	if (material && geometry.VertexCount() > 0){
		size_t vertexElementSize = geometry.VertexSize();
		bool texCoords = geometry.HasTexCoords();

		GLsizei vertexStride = texCoords ? (vertexElementSize + 2) * sizeof (GLfloat) : 0;

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		SetActiveMaterial(material);

		GLint programId = material->Shader()->ProgramId();
		GLuint gPositionLoc = glGetAttribLocation ( programId, "recPosition" );
		GLuint gMatrixLoc = glGetUniformLocation ( programId, "recMVPMatrix" );
		GLuint gTexCoordLoc = 0;
		
		glUniformMatrix4fv(gMatrixLoc, 1, GL_FALSE, transform.m);

		glVertexAttribPointer ( gPositionLoc, vertexElementSize, GL_FLOAT, GL_FALSE, vertexStride, geometry.VertexData() );
		glEnableVertexAttribArray ( gPositionLoc );

		if (texCoords){
			gTexCoordLoc = glGetAttribLocation ( programId, "recTexCoord" );
			glVertexAttribPointer ( gTexCoordLoc, 2, GL_FLOAT, GL_FALSE, vertexStride, geometry.VertexData() + (vertexElementSize * sizeof (GLfloat)));
			glEnableVertexAttribArray ( gTexCoordLoc );
		}
		glDrawElements ( GLGeometryType(geometry.GeometryType()), geometry.IndexCount(), GL_UNSIGNED_SHORT, geometry.IndexData() );

		glDisableVertexAttribArray ( gPositionLoc );

		if (texCoords)
			glDisableVertexAttribArray ( gTexCoordLoc );

	}
}

GLenum rOpenGLGraphicsDevice::GLGeometryType(rGeometryType type) const{
	switch (type){
	case rGeometryType::LINES:
		return GL_LINES;
	case rGeometryType::LINE_LOOP:
		return GL_LINE_LOOP;
	case rGeometryType::POINTS:
		return GL_POINTS;
	default:
		return GL_TRIANGLES;
	}
}

unsigned int rOpenGLGraphicsDevice::CreateRenderbuffer(int width, int height){
	rglRenderbuffer renderbuffer;
	renderbuffer.width = width;
	renderbuffer.height = height;
	
	glGenFramebuffers(1, &renderbuffer.framebufferId);
	glGenRenderbuffers(1, &renderbuffer.renderbufferId);
	glGenRenderbuffers(1, &renderbuffer.depthBufferId);

	glBindFramebuffer(GL_FRAMEBUFFER, renderbuffer.framebufferId);
	

	//todo: opengl es: GL_RGBA8_OES, GL_COLOR_ATTACHMENT0_OES? etc...
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer.renderbufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer.renderbufferId);

	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer.depthBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer.depthBufferId);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status == GL_FRAMEBUFFER_COMPLETE) {
		unsigned int targetId = ++m_nextRenderbufferId;

		m_renderBuffers[targetId] = renderbuffer;
		m_activeRenderBufferId = targetId;
		return targetId;
    }
	else{
		rLog::Error("Error Creating Offscreen Renderbuffer: %d", status);
        return 0;
	}
}

void rOpenGLGraphicsDevice::DeleteRenderbuffer(unsigned int id){
	if (m_renderBuffers.count(id)){
		rglRenderbuffer& renderbuffer = m_renderBuffers[id];

		glDeleteRenderbuffers(1, &renderbuffer.renderbufferId);
		glDeleteRenderbuffers(1, &renderbuffer.depthBufferId);
		glDeleteFramebuffers(1, &renderbuffer.framebufferId);
		

		glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFramebuffer);
		m_activeRenderBufferId = 0;
	}
}

unsigned int rOpenGLGraphicsDevice::ReadRenderbufferPixel(unsigned int x, unsigned int y){
	rglRenderbuffer& renderbuffer = m_renderBuffers[m_activeRenderBufferId];
	y = renderbuffer.height - y;

	unsigned char color[4] = {255,255,255,255};
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	return 0;
}