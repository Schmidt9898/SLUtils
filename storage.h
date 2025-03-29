#ifndef SLU_STORAGE
#define SLU_STORAGE

#include <EEPROM.h>

// Define EEPROM size based on the detected board
#if defined(__AVR_ATmega328P__) // Arduino Uno, Nano
#define EEPROM_SIZE 1024
#elif defined(__AVR_ATmega2560__) // Arduino Mega 2560
#define EEPROM_SIZE 4096
#elif defined(__AVR_ATmega32U4__) // Arduino Leonardo
#define EEPROM_SIZE 1024
#elif defined(ESP32)	// ESP32 (uses flash memory, emulated EEPROM)
#define EEPROM_SIZE 512 // Default size (can be changed in EEPROM.begin(size))
#else
#define EEPROM_SIZE 0 // Unknown board, no EEPROM defined
#warning "EEPROM size is set to 0. Unknown board detected."
#endif

uint8_t CalculateTimeHash(void)
{
	char *str = __TIME__;
	uint8_t sum = 0;
	while (*str)
	{
		if (isAlphaNumeric(*str))
		{
			sum += isAlphaNumeric(*str) ? *str - '0' : *str;
		}
		str++; // Move to the next character
	}
	return sum;
};

uint8_t randomCompileSeedID = CalculateTimeHash(); // Generated at compile time.

class LoadSaveListNode
{
public:
	LoadSaveListNode *next = nullptr;
	virtual void load() = 0;
	virtual void save() = 0;
	virtual LoadSaveListNode() {};
	virtual ~LoadSaveListNode() {};
};
LoadSaveListNode *_LoadSaveListEntry = nullptr;

template <typename T>
class NonVolatileData : public LoadSaveListNode
{
private:
	int flash_data_ptr = -1;

public:
	T data;
	NonVolatileData(int _flash_data_ptr) : flash_data_ptr(_flash_data_ptr)
	{
		if (_LoadSaveListEntry == nullptr)
		{
			_LoadSaveListEntry = this;
		}
		else
		{
			this->next = _LoadSaveListEntry;
			_LoadSaveListEntry = this;
		}
		load();
	}
	void save()
	{
		EEPROM.put(flash_data_ptr, data);
		print("Saving data to EEPROM at address: ");
		println(flash_data_ptr, DEC);
	}
	void load()
	{
		EEPROM.get(flash_data_ptr, data);
		print("Loading data from EEPROM at address: ");
		println(flash_data_ptr, DEC);
	}
	~NonVolatileData() {};
};

#define STORED_DATA(type, name)        \
	volatile type EEMEM _flash_##name; \
	NonVolatileData<type> name = NonVolatileData<type>(int(&_flash_##name));

struct SluManagementData
{
	uint8_t init_value;
	int counter = 0;
};

/**
 * @brief This value holds the Slu management data in the EEPROM.
 *
 */
STORED_DATA(SluManagementData, slu_management_data);

void SluSaveAllData()
{
	LoadSaveListNode *current = _LoadSaveListEntry;
	while (current != nullptr)
	{
		current->save();
		current = current->next;
	}
}
/**
 * @brief Checks if this is the first run of the program.
 * @details First call discards the first run flag, resulting every other call to return false.
 *
 * @return true This is the first run after uploading the program.
 * @return false This program is already ran at least once.
 */
bool SluIsFirstRun()
{
	if (slu_management_data.data.init_value != randomCompileSeedID)
	{
		slu_management_data.data.init_value = randomCompileSeedID;
		return true;
	}
	return false;
}

#endif
