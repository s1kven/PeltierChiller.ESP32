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
            updateAndApplyConfiguration = 52,
            updateTempConfiguration = 53,
            resetTempConfiguration = 54,
            //errors
            errorConfiguration = 100,
            errorRequest = 101,
            errorDeserialization = 102,
            errorSd = 103
        };
    }
}