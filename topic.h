#ifndef TOPIC_H
#define TOPIC_H

class topic
{
public:
	size_t id, frequency;

	topic():
		id(0), frequency(0)
	{ }

	topic(size_t in_id, size_t in_freq):
		id(in_id), frequency(in_freq)
	{ }
};
#endif