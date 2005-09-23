/* BasicEditorPaneUI.java -- 
   Copyright (C) 2004  Free Software Foundation, Inc.

This file is part of GNU Classpath.

GNU Classpath is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Classpath is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Classpath; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */


package javax.swing.plaf.basic;

import javax.swing.JComponent;
import javax.swing.JEditorPane;
import javax.swing.plaf.ComponentUI;
import javax.swing.text.EditorKit;
import javax.swing.text.Element;
import javax.swing.text.JTextComponent;
import javax.swing.text.PlainView;
import javax.swing.text.View;

/**
 * The UI class for  {@link JEditorPane}s.
 *
 * @author original author unknown
 * @author Roman Kennke (roman@kennke.org)
 */
public class BasicEditorPaneUI extends BasicTextUI
{
  /**
   * Creates an instance of <code>BasicEditorPaneUI</code> for the text
   * component <code>comp</code>.
   *
   * @param comp the component for which to create an UI
   *
   * @return the UI for <code>comp</code>
   */
  public static ComponentUI createUI(JComponent comp)
  {
    return new BasicEditorPaneUI();
  }

  /**
   * Creates a new <code>BasicEditorPaneUI</code>
   */
  public BasicEditorPaneUI()
  {
    // Do nothing here.
  }

  // FIXME: Should not be overridden here but instead be handled by the
  // JEditorPane's EditorKit. However, as long as we don't have styles in
  // place this doesn't make much sense.
  public View create(Element elem)
  {
    return new PlainView(elem);
  }

  /**
   * Returns the property prefix to be used by this UI class. This is
   * <code>EditorPane</code> in this case.
   *
   * @return <code>EditorPane</code>
   */
  protected String getPropertyPrefix()
  {
    return "EditorPane";
  }

  /**
   * Gets the EditorKit for the text component.
   *
   * @param textComponent the text component for which to fetch the editor kit
   */
  public EditorKit getEditorKit(JTextComponent textComponent)
  {
    return ((JEditorPane) textComponent).getEditorKit();
  }
}
