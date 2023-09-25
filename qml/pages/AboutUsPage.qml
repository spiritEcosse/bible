import QtQuick 2.0
import Sailfish.Silica 1.0

Pages {
    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height + Theme.paddingLarge
        contentWidth: parent.width
        VerticalScrollDecorator {}

        Column {
            id: column
            x: Theme.paddingMedium
            spacing: Theme.paddingLarge
            width: parent.width

            PageHeader {
                title: qsTr("About us")
            }

            Column {
                width: parent.width

                Label {
                    text: '
<!DOCTYPE html>
<html>

<body>
    <header>
        <h1>FreeBible</h1>
        <h3>The FreeBible application is completely free for learning and daily reading, and it is designed for everyone.</h3>
    </header>

    <div id="about">
        <h2>About FreeBible</h2>
        <h3>Meet the Developer</h3>
        <p>Greetings, I\'m Ihor Shevchenko 🌟</p>
        <p>I\'m the driving force behind FreeBible, and I\'m thrilled to share my vision for this app with you. My journey is one
            fueled by a passion for making the Bible accessible to everyone, anytime, and anywhere.</p>
        <h3>Who do I believe in?</h3>
        <p>I am Christian, and I believe in the Triune God: Father, Son, and Holy Spirit.</p>
    </div>

    <div id="vision">
        <h2>My Vision</h2>
        <p><strong>FreeBible</strong> was conceived from a dream to make the Bible readily available on any device, in multiple languages,
            with diverse commentaries, all while being completely offline, free, and free from ads. This dream stemmed from my
            desire to read the Bible on my Sailfish device and various other platforms, enabling easy sharing of thoughts and
            fostering a deeper understanding of God\'s profound teachings.</p>
    </div>

    <div id="mission">
        <h2>My Mission</h2>
        <p>At FreeBible, my mission is inspired by <strong>JOSHUA 1:8</strong>:</p>
        <blockquote>
            <p>\'This book of the law shall not depart out of thy mouth; but thou shalt meditate therein day and night, that thou mayest observe to do according to all that is written therein: for then thou shalt make thy way prosperous, and then thou shalt have good success.\'</p>
        </blockquote>
    </div>

    <div id="what-is-freebible">
        <h2>What is FreeBible?</h2>
        <p><strong>FreeBible</strong> is an application designed to empower users with the ability to read the Bible effortlessly, offering
            a multitude of features:</p>
        <ul>
            <li>Download Modules: Access diverse translations with ease.</li>
            <li>Remove Modules: Customize your Bible library as per your preferences.</li>
            <li>Read Offline: Enjoy uninterrupted reading, even without an internet connection.</li>
            <li>Reading History: Easily track your reading progress.</li>
            <li>Commentaries: Gain insights with small commentaries linked to Bible verses.</li>
            <li>Search Functionality: Seamlessly search for specific verses across translations.</li>
        </ul>
    </div>

    <div id="compatibility">
        <h2>Compatibility</h2>
        <p>This project has been tested on the following versions of Sailfish OS:</p>
        <ul>
            <li>4.5.0.19</li>
        </ul>
    </div>

    <div id="todo">
        <h2>TODO</h2>
        <ul>
            <li>Adding settings</li>
            <li>Support commentaries</li>
            <li>Support cover page for Sailfish OS
                <ul>
                    <li>display a spinner during the post-installation process following the application installation</li>
                    <li>show status of loading list of modules</li>
                    <li>show status of loading modules</li>
                    <li>show count of loading modules</li>
                    <li>show verses</li>
                    <li>add carousel for 3 pages</li>
                </ul>
            </li>
            <li>Adding bookmarks</li>
            <li>Adding dictionaries</li>
            <li>Adding notes</li>
            <li>Adding audio notes</li>
            <li>Convert audio to text</li>
            <li>Auditory presentation of textual information</li>
            <li>Adding fonts</li>
            <li>Support strong numbers</li>
            <li>Adding notifications</li>
        </ul>
    </div>

    <div id="support-platforms">
        <h2>Support all modern platforms</h2>
        <ul>
            <li>Linux</li>
            <li>Mac OS</li>
            <li>Windows</li>
            <li>Android</li>
            <li>iOS</li>
        </ul>
    </div>

    <div id="responsive-design">
        <h2>Responsive Design</h2>
        <ul>
            <li>Tablet</li>
            <li>Desktop</li>
            <li>Mobile</li>
        </ul>
    </div>

    <div id="configurations-architectures">
        <h2>Embrace all popular configurations architectures</h2>
    </div>

    <div id="contribute">
        <h2>Contribute to FreeBible</h2>
        <p>If you have experience in UI design, programming skills, or simply carry a spark of inspiration for this app, we invite
            you to become a part of my mission. Your involvement can significantly shape the future of FreeBible.</p>
        <p>Whether you\'re a design virtuoso, a coding maestro, or just someone bursting with creative ideas, we wholeheartedly
            welcome your participation and support. Don\'t hesitate to reach out and join me in making FreeBible even better.
            Your contributions are priceless!</p>
        <p>If you come across any problems or have any suggestions, please don\'t hesitate to share them with us on our GitHub page
            at <a href="https://github.com/spiritEcosse/bible/issues">https://github.com/spiritEcosse/bible/issues</a>.</p>
        <p>Alternatively, if you\'d like, you can simply treat me to a coffee.</p>
        <a href="https://www.buymeacoffee.com/shevchenkco" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png"
                alt="Buy Me A Coffee" ></a>
    </div>

    <div id="contact">
        <h2>Contact Me</h2>
        <p>You can contact me via email at <a href="mailto:shevchenkcoigor@gmail.com">shevchenkcoigor@gmail.com</a> for further information
            or inquiries.</p>
    </div>

    <div id="licensing">
        <h2>Licensing</h2>
        <p>This project is released under the GNU General Public License v3.0 (GPL-3.0), a widely used open-source software license.</p>
    </div>

    <div id="special-thanks">
        <h2>Special Thanks</h2>


 <p>I extend my heartfelt gratitude to those who have contributed to the FreeBible project:</p>
        <ul>
            <li><strong>My God</strong>: Because without His invisible cooperation it would be impossible, I guess.</li>
            <li><strong>My wife Oksana Shevchenko</strong>: For her assistance with the app\'s icon and constant inspiration.</li>
            <li><strong>Team of <a href="https://mybible.zone/en/">MyBible</a></strong>: For their agreement to grant me access to mybible modules.</li>
            <li><strong>Contributors of <a href="https://github.com/fnc12/sqlite_orm" >sqlite_orm</a></strong>: For their AGPL-licensed open-source project.</li>
            <li><strong>Contributors of <a href="https://github.com/stachenov/quazip">quazip</a></strong>: For their valuable contributions.</li>
            <li><strong>Contributors of <a href="https://github.com/llvm/llvm-project">llvm</a></strong>: For their work on the LLVM project.</li>
            <li><strong>Contributors of <a href="https://github.com/linux-test-project/lcov">lcov</a></strong>: For their contributions to the Linux Test Project.</li>
            <li><strong>Contributors of <a href="https://github.com/openssl/openssl">openssl</a></strong>: For their work on the OpenSSL project.</li>
            <li><strong>Contributors of <a href="https://curl.se/">curl</a></strong>: For their dedication to the curl project.</li>
            <li><strong>Contributors of <a href="https://github.com/StableCoder/cmake-scripts">cmake-scripts</a></strong>: For their invaluable contributions to the development process.</li>
        </ul>
        <p>Your support and collaboration have played a pivotal role in shaping FreeBible into what it is today.</p>
        <p>Your support has been instrumental in making FreeBible what it is today.</p>
        <p>I deeply appreciate your commitment to this project.</p>
        <p>Thank you for joining us on this enlightening journey.</p>
        <p>Warm regards,</p>
        <p>Ihor Shevchenko</p>
    </div>
</body>

</html>
'
                    font.pixelSize: Theme.fontSizeMedium
                    wrapMode: Text.WordWrap
                    textFormat: Text.StyledText
                    width: parent.width - Theme.paddingLarge
                    onLinkActivated: Qt.openUrlExternally(link)
                }
            }
        }
    }
}
