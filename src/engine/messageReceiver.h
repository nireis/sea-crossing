#ifndef  messageReceiver_h
#define messageReceiver_h

#include <queue>
#include <atomic>

#include <glm/glm.hpp>
#include <glm/core/type_vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum messageType { CREATE, DELETE, EXIT };

struct Message
{
	Message(messageType in_type, int in_id)
			: type(in_type), id(in_id), position(), orientation(),
				scaling(), geometry_path(), material_path() {}
	Message(messageType in_type,
			int in_id,
			glm::vec3 in_position,
			glm::quat in_orientation,
			glm::vec3 in_scaling,
			std::string in_geometry_path,
			std::string in_material_path)
			: type(in_type), id(in_id), position(in_position), orientation(in_orientation),
				scaling(in_scaling), geometry_path(in_geometry_path), material_path(in_material_path) {}

	messageType type;
	int id;
	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scaling;
	std::string geometry_path;
	std::string material_path;
};

/**
* \class MessageReceiver
* 
* \brief Allows primitive communication
* 
* This class offers a simple form of communication between engine modules via a message FIFO
* 
* \author Michael Becher
* 
* \date 15th August 2013
*/
class MessageReceiver
{
public:
	MessageReceiver() {}
	~MessageReceiver() {}

	void pushCreateMarkerMessage(int id, int player);
	void pushCreateGemMessage(int id, int player);
	void pushDeleteMessage(int id);
	void pushExitMessage();
	Message popMessage(); 

private:
	std::queue<Message> message_fifo;
	std::atomic<unsigned int> message_counter;
};

#endif
