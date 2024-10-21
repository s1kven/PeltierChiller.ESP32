namespace Communication
{
    namespace Enums
    {
        enum class ResponseType
        {
            unknown = 0,
            temperatureSensors = 1,
            pwms = 2,
            //errors
            errorConfiguration = 100
        };
    }
}