import matplotlib.pyplot as plt

# Define x and y values
x_values = [3, 6, 9, 12]
y_values = [515.069467, 291.718987, 211.175244, 185.989028]

# Plot the line graph
plt.figure(figsize=(8, 6))
plt.plot(x_values, y_values, marker='o', linestyle='-', color='blue', label='Data')

# Customize plot
plt.title('Error Plot')
plt.xlabel('M')
plt.ylabel('Y-axis')
plt.legend()

# Save or show the plot
plt.savefig('Error_plot.png')  # Save the plot as an image
plt.show()  # Show the plot
