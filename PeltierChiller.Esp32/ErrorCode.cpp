namespace Communication
{
    namespace Enums
    {
        enum class ErrorCode {
            ok,
            // read configuration
            emptyInput,
            incompleteInput,
            invalidInput,
            noMemory,
            tooDeep,
            // validate configuration
            emptySensors,
            emptyColdCircuitSensors,
            emptyRoomSensors,
            invalidBmeConfiguration,
            //files
            failedWriteFile,
            //errorCommand
            invalidCommand
        };
    }
}