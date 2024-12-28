namespace Communication
{
    namespace Enums
    {
        enum class ResponseType
        {
            unknown = 0,
            temperatureSensors = 1,
            pwms = 2,
            //commands
            softReset = 50,
            updateConfiguration = 51,
            //errors
            errorConfiguration = 100,
            errorCommand = 101,
            errorDeserialization = 102,
            errorSd = 103
        };
    }
}