namespace Communication
{
    namespace Enums
    {
        enum class RequestType
        {
            unknown = 0,
            configuration = 1,
            //commands
            softReset = 50,
            updateConfiguration = 51,
            updateAndApplyConfiguration = 52,
            updateTempConfiguration = 53,
            resetTempConfiguration = 54,
            //errors
            errorDeserialization = 101
        };
    }
}