# **Tomeo Video Player Prototype - README**

---

## **Project Overview**

**Tomeo** is a video player prototype developed using **Qt 5.14**, designed to deliver an efficient, convenient, and feature-rich video playback experience. By implementing core playback controls, user interaction features (like favorites, likes, and comments), multi-language support, and responsive design, the player offers an intelligent and user-friendly video viewing experience. The project follows **Agile Development Methodology**, continuously refining functionality and user interface through iterative development.

---

## **Features**

### 1. **Video File Management and Playback**

- **File Loading and Playlist**  
  - Users can click the **Open Folder** button to select a folder containing video files.  
  - The player automatically loads all video files and displays their **thumbnails** and file names in the playlist.  
  - Command-line arguments are supported for specifying the video folder path at startup.

- **Playback Controls**  
  - **Play / Pause**: Quickly toggle video playback by clicking the play button or the video area.  
  - **Progress Control**:  
    - Drag the **progress bar** to adjust the playback position.  
    - Use **5-second fast forward** and **rewind buttons** for precise position adjustment.  
  - **Previous / Next Video**: Switch between videos in the playlist using dedicated buttons.  
  - **Volume Adjustment**: Use the volume slider to adjust the video playback volume.  
  - **Fullscreen Mode**: Click the **fullscreen button** to enter immersive fullscreen mode. Press again to exit.

---

### 2. **User Interaction Features**

- **Like and Favorite**  
  - Users can **like** or **favorite** a video using dedicated buttons.  
  - Both features act as simple toggle states, and the current status is displayed on the interface.  

- **Comments**  
  - Users can add comments to a video by entering a username and comment content.  
  - Existing comments can be **edited** or **deleted**.  
  - All interaction data (likes, favorites, and comments) are stored persistently in a `data.json` file.

**Sample `data.json` (snippet):**  
```json
{
    "a.wmv": {
        "collected": true,
        "liked": true
    },
    "b.wmv": {
        "collected": false,
        "comments": [
            {"id": 1, "username": "User1", "content": "Nice video!"}
        ],
        "liked": false
    }
}
```

---

### 3. **Playback Modes**

The player supports the following four playback modes to suit diverse user preferences:  

1. **List Loop**: Continuously play all videos in the playlist.  
2. **Single Loop**: Repeat the current video.  
3. **Random Loop**: Randomly play videos from the playlist.  
4. **No Loop**: Stop playback after the current video finishes.  

Users can switch playback modes easily using the mode selection button, and the current mode is displayed in the interface.

---

### 4. **Responsive Design and Multi-language Support**

#### **Responsive Design**  
- Optimized for small screens by providing a **vertical panel layout**, maximizing interaction space.  
- Non-essential controls are automatically hidden on smaller screens to maintain a clean interface.  
- Users can tap the video area to **play / pause** the video directly, enabling a smoother experience.

#### **Multi-language Support**  
- A **language toggle button** allows users to switch between **Chinese** and **English**.  
- All interface text and messages are updated dynamically based on the selected language, ensuring internationalization.

---

## **Technical Implementation**

- **Qt 5.14**: Provides the core framework for functionality and user interface development.  
- **Qt Designer**: Used for designing and laying out the graphical user interface.  
- **C++**: Handles playback logic, user interaction, and data management.  
- **JSON**: Used for storing persistent user data, such as likes, favorites, and comments.

---

## **Installation and Setup**

### **System Requirements**  
- **Operating System**: Windows, MacOS, or Linux  
- **Qt Version**: Qt 5.14  
- **Compiler**: C++17 or higher  

### **Installation Steps**  
1. **Clone the Repository**  
   ```bash
   git clone https://github.com/lcx-0504/UserInterfaceCoursework.git
   ```

2. **Import the Project**  
   - Open the project folder using **Qt Creator**.  

3. **Build and Run**  
   - Compile the project and run it.  
   - Use the **Open Folder** button to load a video folder and start playing videos.

---

## **Future Plans**

1. **Dark and Light Mode Switching**  
   - Introduce a toggle between **dark mode** and **light mode** to provide a customizable viewing experience.

2. **History and Video Recommendations**  
   - Record user viewing history and suggest related videos based on user preferences.

3. **Mobile Adaptation**  
   - Optimize layout and functionality for mobile devices.

4. **Cloud Synchronization**  
   - Enable cloud-based storage for user data, ensuring seamless multi-device access.

---

## **Contributing and Feedback**

We welcome all suggestions and contributions!  
- **GitHub Issues**: [Submit issues and suggestions](https://github.com/lcx-0504/UserInterfaceCoursework/issues)  
- **Contact Email**: `support@tomeo.com`  

---

**Tomeo**: Making video playback smarter, more efficient, and more enjoyable!  