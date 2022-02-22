/* Top-level window containing a simple text editor or viewer.

 Copyright (c) 1998-2016 The Regents of the University of California.
 All rights reserved.
 Permission is hereby granted, without written agreement and without
 license or royalty fees, to use, copy, modify, and distribute this
 software and its documentation for any purpose, provided that the above
 copyright notice and the following two paragraphs appear in all copies
 of this software.

 IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
 FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
 THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.

 THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
 PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
 CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 ENHANCEMENTS, OR MODIFICATIONS.

 PT_COPYRIGHT_VERSION_2
 COPYRIGHTENDKEY

 */
package ptolemy.actor.gui.syntax;

import java.awt.BorderLayout;
import java.awt.Toolkit;
import java.awt.event.KeyEvent;

import javax.swing.JTextArea;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.text.Document;

import org.fife.ui.rsyntaxtextarea.ErrorStrip;
import org.fife.ui.rsyntaxtextarea.RSyntaxDocument;
import org.fife.ui.rsyntaxtextarea.RSyntaxTextArea;
import org.fife.ui.rsyntaxtextarea.RSyntaxTextAreaEditorKit;
import org.fife.ui.rtextarea.RTextScrollPane;

import diva.gui.GUIUtilities;
import ptolemy.gui.UndoListener;
import ptolemy.kernel.util.Attribute;
import ptolemy.vergil.toolbox.TextEditorFactory;
import ptolemy.vergil.toolbox.TextEditorForStringAttributes;

///////////////////////////////////////////////////////////////////
//// SyntaxTextEditorForStringAttributes

/**

 A text editor to edit a specified string attribute using a syntax-directed editor.

 @author Edward A. Lee
 @version $Id$
 @since Ptolemy II 11.0
 @Pt.ProposedRating Yellow (eal)
 @Pt.AcceptedRating Red (eal)
 */
@SuppressWarnings("serial")
public class SyntaxTextEditorForStringAttributes
        extends TextEditorForStringAttributes {

    /** Create a annotation text editor for the specified attribute.
     *  @param factory The factory that created this editor.
     *  @param attributeToEdit The string attribute to edit.
     *  @param rows The number of rows.
     *  @param columns The number of columns.
     *  @param title The window title to use.
     *  @param document The document
     */
    public SyntaxTextEditorForStringAttributes(TextEditorFactory factory,
            Attribute attributeToEdit, int rows, int columns, String title,
            Document document) {
        super(factory, attributeToEdit, rows, columns, title, document);
    }

    ///////////////////////////////////////////////////////////////////
    ////                         protected methods                 ////

    /** Create an edit menu.
     */
    @Override
    protected void _addMenus() {
        super._addMenus();

        _editMenu.addSeparator();

        GUIUtilities.addMenuItem(_editMenu,
                new RSyntaxTextAreaEditorKit.IncreaseFontSizeAction(
                        "Increase Font", null, "Increase the font size.", 8,
                        KeyStroke.getKeyStroke(KeyEvent.VK_EQUALS,
                                Toolkit.getDefaultToolkit()
                                        .getMenuShortcutKeyMask())));
        GUIUtilities.addMenuItem(_editMenu,
                new RSyntaxTextAreaEditorKit.DecreaseFontSizeAction(
                        "Decrease Font", null, "Decrease the font size.", 9,
                        KeyStroke.getKeyStroke(KeyEvent.VK_MINUS,
                                Toolkit.getDefaultToolkit()
                                        .getMenuShortcutKeyMask())));
    }

    /** Initializes an empty text editor with the specified title and
     *  document and associated placeable.  After constructing this,
     *  it is necessary to call setVisible(true) to make the frame
     *  appear.
     *
     *  @param title The title to put in the title bar.
     *  @param document The document containing text.
     */
    @Override
    protected void _init(final String title, Document document) {
        // No idea why this needs to be invoked later, but if it's invoked now,
        // the title on the window ends up being "Unnamed".
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                setTitle(title);
            }
        });

        if (document instanceof RSyntaxDocument) {
            text = new RSyntaxTextArea((RSyntaxDocument) document);
            // The default tab size is odd: 5.
            text.setTabSize(4);
            text.setCaretPosition(0);
            // ((RSyntaxTextArea)text).addHyperlinkListener(this);
            text.requestFocusInWindow();
            // ((RSyntaxTextArea)text).setMarkOccurrences(true);
            ((RSyntaxTextArea) text).setCodeFoldingEnabled(true);
            ((RSyntaxTextArea) text).setClearWhitespaceLinesEnabled(false);
        } else if (document != null) {
            text = new JTextArea(document);
        } else {
            text = new JTextArea();
        }

        // Since the document may have been null, request it...
        document = text.getDocument();
        document.addDocumentListener(this);
        _scrollPane = new RTextScrollPane(text, true);

        // To get bookmarking, do this:
        //        Gutter gutter = ((RTextScrollPane)_scrollPane).getGutter();
        //        gutter.setBookmarkingEnabled(true);
        //        URL url = getClass().getClassLoader().getResource("img/bookmark.png");
        //        gutter.setBookmarkIcon(new ImageIcon(url));
        // Will need to copy the img/bookmark.png from the rsyntaxtextarea_demo_2.5.1_Source dir.

        ErrorStrip errorStrip = new ErrorStrip((RSyntaxTextArea) text);
        getContentPane().add(errorStrip, BorderLayout.LINE_END);

        getContentPane().add(_scrollPane, BorderLayout.CENTER);
        _initialSaveAsFileName = "data.txt";

        // Set the undo listener, with default key mappings.
        _undo = new UndoListener(text);
        text.getDocument().addUndoableEditListener(_undo);
    }
}
