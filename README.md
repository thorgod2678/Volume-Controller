# Volume-Controller
A simple win32 based volume controller which uses the IAudioEndpoint interface to manage the volume.

The program supports expressions as inputs, and the floating point output obtained is sent to the Windows API to be set as the volume. If the output doesn't match the requirements of the API or is unsupported by the audio device, then the volume remains unchanged.

The program can set the volume both in Decibels or Scalar(the way the volume slider works)

The expression handler supports the following:

                + -> Addition
                - -> Subtraction
                * -> Multiplication
                / -> Division
                ^ -> Exponentiation
                () -> Parentheses for controlling calculation order
                PI -> Mathematical constant Ï€
                E -> Mathematical constant e
                sin() -> Sine
                cos() -> Cosine
                tan() -> Tangent
                csc() -> Cosecant
                sec() -> Secant
                cot() -> Cotangent
                asin() -> Inverse sine
                acos() -> Inverse cosine
                atan() -> Inverse tangent
                sqrt() -> Square root
                abs() -> Absolute value
                floor() -> Rounds down to the nearest whole number
                ceil() -> Rounds up to the nearest whole number

The background color of the window can be changed if needed.

Screenshots of the program:

<img width="411" height="447" alt="image" src="https://github.com/user-attachments/assets/144947b1-c472-4969-bc47-64a9f77d684f" />

<img width="683" height="461" alt="image" src="https://github.com/user-attachments/assets/6766c528-659d-48f5-8e32-1aa6040bfdc6" />
