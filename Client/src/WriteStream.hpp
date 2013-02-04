#ifndef _WRITE_STREAM_HPP_
# define _WRITE_STREAM_HPP_

# include <boost/asio.hpp>
# include <boost/bind.hpp>

template <typename T>
class WriteStream {
public:
	WriteStream(T* object) : _object(object) {}

	void write(int size = 0, void* buffer = NULL) {
		if (size && buffer) {
			char* buf = new char[size];
			memcpy(buf, buffer, size);
			_toWrite.push(std::pair<int, char*>(size, buf));
		}
		if (!_writing) {
			_writing = true;
			async_write(*_object, boost::asio::buffer(_toWrite.front().second, _toWrite.front().first),
						boost::bind(&WriteStream<T>::writeHandler, this, _1, _2));
		}
	}

	void writeHandler(const boost::system::error_code& error, size_t) {
		if (!error) {
			_writing = false;
			delete [] _toWrite.front().second;
			_toWrite.pop();
			this->write();
		}
	}
private:
	T*										_object;
	std::queue<std::pair<int, char*>>		_toWrite;
	bool									_writing;
};

#endif