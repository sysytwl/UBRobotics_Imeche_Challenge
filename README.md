# UBRobotics_Imeche_Challenge

![Alt text](./ESP32-Pinout.png "pinout")

<li><span class="my_icon ex_icon"></span> – Pay close attention because their behavior, particularly during boot, can be unpredictable. Use them only when absolutely necessary.</li>
<li><span class="my_icon cross_icon"></span> – It is recommended that you avoid using these pins.</li>

<table class="tabler pininfo"><tbody><tr class="tabler-header"><td>&nbsp;&nbsp;Label&nbsp;&nbsp;</td><td>&nbsp;&nbsp;GPIO&nbsp;&nbsp;</td><td>&nbsp;&nbsp;Safe&nbsp;to&nbsp;use?&nbsp;&nbsp;</td><td>Reason</td></tr><tr><td>D0</td><td>0</td><td><span class="my_icon ex_icon"></span></td><td>must be HIGH during boot and LOW for programming</td></tr><tr><td>TX0</td><td>1</td><td><span class="my_icon cross_icon"></span></td><td>Tx pin, used for flashing and debugging</td></tr><tr><td>D2</td><td>2</td><td><span class="my_icon ex_icon"></span></td><td>must be LOW during boot and also connected to the on-board LED</td></tr><tr><td>RX0</td><td>3</td><td><span class="my_icon cross_icon"></span></td><td>Rx pin, used for flashing and debugging</td></tr><tr><td>D4</td><td>4</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>D5</td><td>5</td><td><span class="my_icon ex_icon"></span></td><td>must be HIGH during boot</td></tr><tr><td>D6</td><td>6</td><td><span class="my_icon cross_icon"></span></td><td>Connected to Flash memory</td></tr><tr><td>D7</td><td>7</td><td><span class="my_icon cross_icon"></span></td><td>Connected to Flash memory</td></tr><tr><td>D8</td><td>8</td><td><span class="my_icon cross_icon"></span></td><td>Connected to Flash memory</td></tr><tr><td>D9</td><td>9</td><td><span class="my_icon cross_icon"></span></td><td>Connected to Flash memory</td></tr><tr><td>D10</td><td>10</td><td><span class="my_icon cross_icon"></span></td><td>Connected to Flash memory</td></tr><tr><td>D11</td><td>11</td><td><span class="my_icon cross_icon"></span></td><td>Connected to Flash memory</td></tr><tr><td>D12</td><td>12</td><td><span class="my_icon ex_icon"></span></td><td>must be LOW during boot</td></tr><tr><td>D13</td><td>13</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>D14</td><td>14</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>D15</td><td>15</td><td><span class="my_icon ex_icon"></span></td><td>must be HIGH during boot, prevents startup log if pulled LOW</td></tr><tr><td>RX2</td><td>16</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>TX2</td><td>17</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>D18</td><td>18</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>D19</td><td>19</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>D21</td><td>21</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>D22</td><td>22</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>D23</td><td>23</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>D25</td><td>25</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>D26</td><td>26</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>D27</td><td>27</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>D32</td><td>32</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>D33</td><td>33</td><td><span class="my_icon check_icon"></span></td><td></td></tr><tr><td>D34</td><td>34</td><td><span class="my_icon ex_icon"></span></td><td>Input only GPIO, cannot be configured as output</td></tr><tr><td>D35</td><td>35</td><td><span class="my_icon ex_icon"></span></td><td>Input only GPIO, cannot be configured as output</td></tr><tr><td>VP</td><td>36</td><td><span class="my_icon ex_icon"></span></td><td>Input only GPIO, cannot be configured as output</td></tr><tr><td>VN</td><td>39</td><td><span class="my_icon ex_icon"></span></td><td>Input only GPIO, cannot be configured as output</td></tr></tbody></table>
