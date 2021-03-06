#include <Buffalo/Rendering/Program.hpp>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>
#include <Buffalo/Rendering/Shader.hpp>
#include <Buffalo/Rendering/Status.hpp>
#include <Buffalo/Rendering/UniformVariable.hpp>
#include <Buffalo/Core/Debug.hpp>

namespace Buffalo {
	namespace Rendering {
		bool VerifyUniform(const UniformVariable& uniform, UniformDataType type);

		Program::Program()
			: handle(GL::CreateProgram()) {
			BUFFALO_PROFILE_FUNC();
		}

		Program::~Program() {
			BUFFALO_PROFILE_FUNC();
			GL::DeleteProgram(handle);
		}
		
		void Program::AttachShader(Shader& shader) {
			BUFFALO_PROFILE_FUNC();
			if (!shader.IsCompiled()) {
				throw std::runtime_error("Attempted to attach an uncompiled shader.");
			}
			GL::AttachShader(handle, shader.Handle());
		}

		const AttributeVariable& Program::GetAttribute(const std::string& name) {
			BUFFALO_PROFILE_FUNC();
			return attributes[name];
		}

		const UniformVariable& Program::GetUniform(const std::string& name) {
			BUFFALO_PROFILE_FUNC();
			return uniforms[name];
		}

		void Program::DetachShader(Shader& shader) {
			BUFFALO_PROFILE_FUNC();
			GL::DetachShader(handle, shader.Handle());
		}

		std::string Program::GetInfoLog() {
			BUFFALO_PROFILE_FUNC();
			GLint length = 0;
			GL::GetProgramiv(handle, ProgramParameterName::InfoLogLength, &length);
			std::string infoLog(length, ' ');
			GL::GetProgramInfoLog(handle, length, nullptr, &infoLog[0]);
			return infoLog;
		}

		bool Program::IsLinked() {
			BUFFALO_PROFILE_FUNC();
			GLint status = 0;
			GL::GetProgramiv(handle, ProgramParameterName::LinkStatus, &status);
			return static_cast<Status>(status) == Status::Success;
		}

		bool Program::IsUsed() const {
			BUFFALO_PROFILE_FUNC();
			GLint currentProgram = 0;
			GL::GetIntegerv(ParameterName::CurrentProgram, &currentProgram);
			return currentProgram == static_cast<GLint>(handle);
		}

		void Program::Link() {
			BUFFALO_PROFILE_FUNC();
			GL::LinkProgram(handle);

			if (!IsLinked()) {
				std::string infoLog = GetInfoLog();
				throw std::runtime_error(infoLog);
			}

			RefreshAttributes();
			RefreshUniforms();
		}

		void Program::RefreshAttributes() {
			BUFFALO_PROFILE_FUNC();
			GLint count = 0;
			GL::GetProgramiv(handle, ProgramParameterName::ActiveAttributes, &count);
			for (GLint i = 0; i < count; ++i) {
				GLsizei maxNameLength = 64;
				std::string name(maxNameLength, '\0');
				GLsizei length = 0;
				GLint size = 0;
				AttributeDataType type;
				GL::GetActiveAttrib(handle, i, maxNameLength, &length, &size, &type, &name[0]);
				name.resize(length);
				GLint location = GL::GetAttribLocation(handle, name.c_str());

				AttributeVariable v(type, location, name, size);
				attributes.emplace(std::make_pair(name.c_str(), v));
			}
		}

		void Program::RefreshUniforms() {
			BUFFALO_PROFILE_FUNC();
			GLint count = 0;
			GL::GetProgramiv(handle, ProgramParameterName::ActiveUniforms, &count);
			for (GLint i = 0; i < count; ++i) {
				GLsizei maxNameLength = 64;
				std::string name(maxNameLength, '\0');
				GLsizei length = 0;
				GLint size = 0;
				UniformDataType type;
				GL::GetActiveUniform(handle, i, maxNameLength, &length, &size, &type, &name[0]);
				name.resize(length);
				GLint location = GL::GetUniformLocation(handle, name.c_str());

				UniformVariable v(location, type, size, name);
				uniforms.emplace(name.c_str(), v);
			}
		}

		void Program::SetUniform(const UniformVariable& uniform, glm::vec4& v) {
			BUFFALO_PROFILE_FUNC();
			assert(VerifyUniform(uniform, UniformDataType::FloatVec4));
			GL::ProgramUniform4f(handle, uniform.Location, v);
		}

		void Program::SetUniform(const UniformVariable& uniform, const glm::mat4& v) {
			BUFFALO_PROFILE_FUNC();
			assert(VerifyUniform(uniform, UniformDataType::FloatMat4));
			GL::ProgramUniformMatrix4fv(handle, uniform.Location, 1, false, glm::value_ptr(v));
		}

		void Program::Use() {
			BUFFALO_PROFILE_FUNC();
			GL::UseProgram(handle);
		}

		bool VerifyUniform(const UniformVariable& uniform, UniformDataType type) {
			BUFFALO_PROFILE_FUNC();
			if (uniform.Type != type) {
				return false;
			}
			return true;
		}
	}
}