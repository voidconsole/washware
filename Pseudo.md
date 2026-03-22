**Enhanced Washing Machine Logic**

### Updated Components and Controls:

#### Inputs:

1. **Switches:**
   - **Water Level Selector (M):** Cycles through [High, Mid, Low], indicated by:
     - High: Constant LED.
     - Mid: Fast blinking LED.
     - Low: Slow blinking LED.
   - **Time Selector (T):** Cycles through [15 min, 30 min, 45 min], indicated via its LED.
   - **If Soak (S):** Boolean switch with an LED indicator.
   - **Start:** Initiates the washing sequence.
2. **Door Sensor:** Ensures the door is closed before starting.

#### Outputs:

1. **Motor Forward (A):** Spins the drum forward.
2. **Motor Backward (B):** Spins the drum backward.
3. **Drum & Rinse (D):** Activates the drum and rinse mechanism.
4. **Water Valve (C):** Controls water inflow.

#### Indicators:

- LEDs for water level (M), time (T), and soak (S).

### Washing Sequence Logic:

1. **Initialization:**
   
   - Ensure **A**, **B**, **C**, and **D** are OFF.
   - Wait for the door sensor to confirm the door is closed.

2. **Water Filling:**
   
   - Based on **M**:
     - **High:** Turn on **C** for 10 minutes.
     - **Mid:** Turn on **C** for 5 minutes.
     - **Low:** Turn on **C** for 2 minutes.
   - Turn off **C** after the respective duration.

3. **Soaking (If Soak is Enabled):**
   
   - If **S** is ON, wait for 20 minutes before proceeding.

4. **Main Washing Cycle:**
   
   - Define the number of cycles (**k**) as 2 by default.
   - Adjust the total washing time (**T**) to distribute evenly across **k** cycles.
   - For each cycle **i** in **k**:
     - **Wash Stage:**
       - Loop through the following sequence for the allotted cycle time:
         1. Turn on **A** for 2 minutes.
         2. Turn off **A** for 30 seconds.
         3. Turn on **B** for 2 minutes.
         4. Turn off **B** for 30 seconds.
     - **Rinse Stage:**
       1. Turn on **D** for 15 minutes.
       2. Turn all components OFF for 5 seconds.
       3. Turn on both **B** and **D** for 30 seconds.

5. **Final Cycle Adjustments:**
   
   - If **i == k** (last cycle):
     - Before the cycle begins:
       - Based on **M**:
         - **High:** Turn on **C** for 3 minutes.
         - **Mid:** Turn on **C** for 2 minutes.
         - **Low:** Turn on **C** for 1 minute.
       - Turn off **C**.
     - In the **Rinse Stage**:
       - Turn on **D** for 15 minutes.
       - Turn all components OFF for 5 seconds.
       - Turn on both **B** and **D** for 5 minutes.

6. **Program Termination:**
   
   - Reset all components (**A**, **B**, **C**, **D**) to OFF.
   - End the program.

### Arduino Pin Configuration

#### Output Pins

- Pin 12: Motor A (Main wash motor)
- Pin 11: Motor B (Agitator motor) 
- Pin 10: Valve C (Water inlet)
- Pin 9: Pump D (Drain pump)

#### Input Pins

- Pin 7: Water Level Selector (M)
- Pin 6: Time Duration Selector (T)
- Pin 5: Soak Enable Switch (S)
- Pin 4: Start Button
- Pin 8: Door Safety Sensor

#### Status LED Pins

- Pin 3: Water Level Indicator LED
- Pin 2: Time Duration Indicator LED  
- Pin 1: Soak Mode Indicator LED

### Code Structure Recommendations

1. Implement modular functions for:
   
   - Component control (motors, valve, pump)
   - Cycle stages (wash, rinse, drain)
   - Safety checks
   - Input reading and validation
   - Status updates

2. Main program flow should:
   
   - Initialize all pins and components
   - Read and validate inputs
   - Execute washing sequence

3. Code Quality Guidelines:
   
   - Use clear, descriptive function and variable names
   - Break down complex operations into smaller, focused functions
   - Add detailed comments explaining the logic and purpose of each section
   - Use pseudocode to plan complex sequences before implementation
   - Follow consistent naming conventions throughout

4. Function Organization:
   
   - Create separate functions for logically grouped operations
   - Keep functions focused on a single responsibility
   - Use helper functions to avoid code duplication
   - Consider creating utility functions for common operations

5. Documentation Requirements:
   
   - Add header comments for each function describing:
     - Purpose and functionality
     - Input parameters and return values
     - Important assumptions or limitations
   - Include inline comments for complex logic
   - Document any critical timing or sequence dependencies

6. Debug Support:
   
   - Create a dedicated debugging section (initially commented out)
   - Include Serial output statements for:
     - Input detection and validation
     - Component state changes
     - Cycle progression
     - Error conditions
     - Timing information
   - Use consistent formatting for debug messages
