# Emotions2Words  

This project was conceived as a kind of auxiliary device that allows **completely paralyzed people** to type text on a computer and thereby communicate with others.  
Typing on a computer is carried out using **emotion recognition**.  

The video below shows how this happens (video coming soon).  
Currently **only Russian alphabet** is supported.  

**One part** of software responsible for emotion recognition uses **Python** and **OpenVINO toolkit**. [OpenVINO-EmotionRecognition project](https://github.com/PINTO0309/OpenVINO-EmotionRecognition) was taken as a basis. I had to tweak it a little to work with the latest version of OpenVINO.  

OpenVINO is an open-source toolkit for optimizing and deploying deep learning models. It provides boosted deep learning performance for vision, audio, and language models from popular frameworks like TensorFlow, PyTorch, and more.  

OpenVINO - **Get Started** [https://docs.openvino.ai/2022.3/get_started.html](https://docs.openvino.ai/2022.3/get_started.html)  
**Install** OpenVINO™ Runtime on Linux [https://docs.openvino.ai/2022.3/openvino_docs_install_guides_installing_openvino_linux_header.html](https://docs.openvino.ai/2022.3/openvino_docs_install_guides_installing_openvino_linux_header.html)  
**Model Zoo:** Overview of OpenVINO™ Toolkit Pre-Trained Models [https://docs.openvino.ai/2022.3/model_zoo.html](https://docs.openvino.ai/2022.3/model_zoo.html)  
OpenVINO on **GitHub:** [https://github.com/openvinotoolkit](https://github.com/openvinotoolkit)  

**Another part** of the software responsible for the virtual keyboard is written in C++/Qt.  

In order **to connect these two parts**, a virtual serial interface (**tty**) and **socat** software are used.  
How to **install socat** on Ubuntu:  
```
    sudo apt-get install socat
```  
How to run software:

 - run socat: `sudo socat -d -d pty,link=/dev/tty0,raw,echo=0 pty,link=/dev/tty1,raw,echo=0`
 - run emotion recognition: `sudo python ./emotion-recognition/main.py`
 - run virtual keyboard with sudo and choose tty1 port.
