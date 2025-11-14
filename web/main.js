"use strict";
// This file is the compiled output of main.ts
// 1. A function to update a DOM element's text content
function updateText(elementId, text) {
    var element = document.getElementById(elementId);
    if (element) {
        element.textContent = text;
    }
    else {
        console.warn("Element with ID '".concat(elementId, "' not found."));
    }
}
// 2. A function to update the image source
function updateImage(elementId, newSrc) {
    var imageElement = document.getElementById(elementId);
    if (imageElement) {
        imageElement.src = newSrc;
    }
    else {
        console.warn("Image element with ID '".concat(elementId, "' not found."));
    }
}
// 3. Main function to run when the document is loaded
function main() {
    console.log("Web Viewer Initialized");
    // Update the text fields with our sample data
    updateText("status", "Loaded (Static)");
    updateText("resolution", "1280x720 (Sample)");
    updateText("fps", "30 (Target)");
    updateText("filter", "Grayscale (via GLSL Shader)");
}
// 4. Wait for the DOM to be fully loaded before running the main function
document.addEventListener("DOMContentLoaded", main);