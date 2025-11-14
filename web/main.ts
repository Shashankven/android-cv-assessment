// This file demonstrates comfort with TypeScript project setup and DOM updates.
// It finds elements in the HTML and updates them.

// 1. A function to update a DOM element's text content
function updateText(elementId: string, text: string): void {
    const element = document.getElementById(elementId);
    if (element) {
        element.textContent = text;
    } else {
        console.warn(`Element with ID '${elementId}' not found.`);
    }
}

// 2. A function to update the image source
function updateImage(elementId: string, newSrc: string): void {
    const imageElement = document.getElementById(elementId) as HTMLImageElement;
    if (imageElement) {
        imageElement.src = newSrc;
    } else {
        console.warn(`Image element with ID '${elementId}' not found.`);
    }
}

// 3. Main function to run when the document is loaded
function main(): void {
    console.log("TypeScript Web Viewer Initialized");

    // Update the text fields with our sample data
    updateText("status", "Loaded (Static)");
    updateText("resolution", "1280x720 (Sample)");
    updateText("fps", "30 (Target)"); // Changed from 15 to 30
    updateText("filter", "Grayscale (via GLSL Shader)");

    // (Optional) Update the image to a static one.
    // We can use a placeholder, or you can save a screenshot from your app and point to it.
    // For now, the placeholder in the HTML is fine.
    // updateImage("processed-image", "path/to/your/screenshot.png");
}

// 4. Wait for the DOM to be fully loaded before running the main function
document.addEventListener("DOMContentLoaded", main);