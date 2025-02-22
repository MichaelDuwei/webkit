/*
 * (C) 1999 Lars Knoll (knoll@kde.org)
 * (C) 2000 Gunnstein Lye (gunnstein@netcom.no)
 * (C) 2000 Frederik Holljen (frederik.holljen@hig.no)
 * (C) 2001 Peter Kelly (pmk@post.com)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef Range_h
#define Range_h

#include "ExceptionCodePlaceholder.h"
#include "FloatRect.h"
#include "FragmentScriptingPermission.h"
#include "IntRect.h"
#include "RangeBoundaryPoint.h"
#include <wtf/Forward.h>
#include <wtf/RefCounted.h>
#include <wtf/Vector.h>

namespace WebCore {

class ClientRect;
class ClientRectList;
class ContainerNode;
class Document;
class DocumentFragment;
class FloatQuad;
class Node;
class NodeWithIndex;
class Text;
#if PLATFORM(IOS)
class SelectionRect;
class VisiblePosition;
#endif

class Range : public RefCounted<Range> {
public:
    WEBCORE_EXPORT static PassRefPtr<Range> create(Document&);
    WEBCORE_EXPORT static PassRefPtr<Range> create(Document&, PassRefPtr<Node> startContainer, int startOffset, PassRefPtr<Node> endContainer, int endOffset);
    WEBCORE_EXPORT static PassRefPtr<Range> create(Document&, const Position&, const Position&);
    WEBCORE_EXPORT static PassRefPtr<Range> create(ScriptExecutionContext&);
#if PLATFORM(IOS)
    // FIXME: Consider making this a static non-member, non-friend function.
    WEBCORE_EXPORT static PassRefPtr<Range> create(Document&, const VisiblePosition&, const VisiblePosition&);
#endif
    WEBCORE_EXPORT ~Range();

    Document& ownerDocument() const { return const_cast<Document&>(m_ownerDocument.get()); }

    WEBCORE_EXPORT Node* startContainer() const { return m_start.container(); }
    WEBCORE_EXPORT int startOffset() const { return m_start.offset(); }
    WEBCORE_EXPORT Node* endContainer() const { return m_end.container(); }
    WEBCORE_EXPORT int endOffset() const { return m_end.offset(); }

    Node* startContainer(ExceptionCode&) const;
    int startOffset(ExceptionCode&) const;
    Node* endContainer(ExceptionCode&) const;
    int endOffset(ExceptionCode&) const;
    WEBCORE_EXPORT bool collapsed(ExceptionCode&) const;

    WEBCORE_EXPORT Node* commonAncestorContainer(ExceptionCode&) const;
    static Node* commonAncestorContainer(Node* containerA, Node* containerB);
    WEBCORE_EXPORT void setStart(PassRefPtr<Node> container, int offset, ExceptionCode& = ASSERT_NO_EXCEPTION);
    WEBCORE_EXPORT void setEnd(PassRefPtr<Node> container, int offset, ExceptionCode& = ASSERT_NO_EXCEPTION);
    WEBCORE_EXPORT void collapse(bool toStart, ExceptionCode&);
    WEBCORE_EXPORT bool isPointInRange(Node* refNode, int offset, ExceptionCode&);
    short comparePoint(Node* refNode, int offset, ExceptionCode&) const;
    enum CompareResults { NODE_BEFORE, NODE_AFTER, NODE_BEFORE_AND_AFTER, NODE_INSIDE };
    CompareResults compareNode(Node* refNode, ExceptionCode&) const;
    enum CompareHow { START_TO_START, START_TO_END, END_TO_END, END_TO_START };
    WEBCORE_EXPORT short compareBoundaryPoints(CompareHow, const Range* sourceRange, ExceptionCode&) const;
    static short compareBoundaryPoints(Node* containerA, int offsetA, Node* containerB, int offsetB, ExceptionCode&);
    static short compareBoundaryPoints(const RangeBoundaryPoint& boundaryA, const RangeBoundaryPoint& boundaryB, ExceptionCode&);
    WEBCORE_EXPORT bool boundaryPointsValid() const;
    bool intersectsNode(Node* refNode, ExceptionCode&);
    void deleteContents(ExceptionCode&);
    PassRefPtr<DocumentFragment> extractContents(ExceptionCode&);
    PassRefPtr<DocumentFragment> cloneContents(ExceptionCode&);
    void insertNode(PassRefPtr<Node>, ExceptionCode&);
    String toString(ExceptionCode&) const;

    String toHTML() const;
    WEBCORE_EXPORT String text() const;

    PassRefPtr<DocumentFragment> createContextualFragment(const String& html, ExceptionCode&);

    void detach(ExceptionCode&);
    WEBCORE_EXPORT PassRefPtr<Range> cloneRange(ExceptionCode&) const;

    WEBCORE_EXPORT void setStartAfter(Node*, ExceptionCode& = ASSERT_NO_EXCEPTION);
    WEBCORE_EXPORT void setEndBefore(Node*, ExceptionCode& = ASSERT_NO_EXCEPTION);
    WEBCORE_EXPORT void setEndAfter(Node*, ExceptionCode& = ASSERT_NO_EXCEPTION);
    WEBCORE_EXPORT void selectNode(Node*, ExceptionCode& = ASSERT_NO_EXCEPTION);
    WEBCORE_EXPORT void selectNodeContents(Node*, ExceptionCode&);
    void surroundContents(PassRefPtr<Node>, ExceptionCode&);
    WEBCORE_EXPORT void setStartBefore(Node*, ExceptionCode&);

    const Position startPosition() const { return m_start.toPosition(); }
    const Position endPosition() const { return m_end.toPosition(); }
    WEBCORE_EXPORT void setStart(const Position&, ExceptionCode& = ASSERT_NO_EXCEPTION);
    WEBCORE_EXPORT void setEnd(const Position&, ExceptionCode& = ASSERT_NO_EXCEPTION);

    WEBCORE_EXPORT Node* firstNode() const;
    WEBCORE_EXPORT Node* pastLastNode() const;

    ShadowRoot* shadowRoot() const;

    enum RangeInFixedPosition {
        NotFixedPosition,
        PartiallyFixedPosition,
        EntirelyFixedPosition
    };

    // Not transform-friendly
    void textRects(Vector<IntRect>&, bool useSelectionHeight = false, RangeInFixedPosition* = 0) const;
    WEBCORE_EXPORT IntRect boundingBox() const;

    // Transform-friendly
    WEBCORE_EXPORT void textQuads(Vector<FloatQuad>&, bool useSelectionHeight = false, RangeInFixedPosition* = 0) const;
    void getBorderAndTextQuads(Vector<FloatQuad>&) const;
    WEBCORE_EXPORT FloatRect boundingRect() const;
#if PLATFORM(IOS)
    WEBCORE_EXPORT void collectSelectionRects(Vector<SelectionRect>&);
#endif

    void nodeChildrenChanged(ContainerNode&);
    void nodeChildrenWillBeRemoved(ContainerNode&);
    void nodeWillBeRemoved(Node*);

    void textInserted(Node*, unsigned offset, unsigned length);
    void textRemoved(Node*, unsigned offset, unsigned length);
    void textNodesMerged(NodeWithIndex& oldNode, unsigned offset);
    void textNodeSplit(Text* oldNode);

    // Expand range to a unit (word or sentence or block or document) boundary.
    // Please refer to https://bugs.webkit.org/show_bug.cgi?id=27632 comment #5 
    // for details.
    void expand(const String&, ExceptionCode&);

    PassRefPtr<ClientRectList> getClientRects() const;
    PassRefPtr<ClientRect> getBoundingClientRect() const;

#ifndef NDEBUG
    void formatForDebugger(char* buffer, unsigned length) const;
#endif

    WEBCORE_EXPORT bool contains(const Range&) const;

private:
    explicit Range(Document&);
    Range(Document&, PassRefPtr<Node> startContainer, int startOffset, PassRefPtr<Node> endContainer, int endOffset);

    void setDocument(Document&);

    Node* checkNodeWOffset(Node*, int offset, ExceptionCode&) const;
    void checkNodeBA(Node*, ExceptionCode&) const;
    void checkDeleteExtract(ExceptionCode&);
    bool containedByReadOnly() const;

    enum ActionType { Delete, Extract, Clone };
    PassRefPtr<DocumentFragment> processContents(ActionType, ExceptionCode&);
    static PassRefPtr<Node> processContentsBetweenOffsets(ActionType, PassRefPtr<DocumentFragment>, Node*, unsigned startOffset, unsigned endOffset, ExceptionCode&);
    static void processNodes(ActionType, Vector<RefPtr<Node>>&, PassRefPtr<Node> oldContainer, PassRefPtr<Node> newContainer, ExceptionCode&);
    enum ContentsProcessDirection { ProcessContentsForward, ProcessContentsBackward };
    static PassRefPtr<Node> processAncestorsAndTheirSiblings(ActionType, Node* container, ContentsProcessDirection, PassRefPtr<Node> clonedContainer, Node* commonRoot, ExceptionCode&);

    Ref<Document> m_ownerDocument;
    RangeBoundaryPoint m_start;
    RangeBoundaryPoint m_end;
};

PassRefPtr<Range> rangeOfContents(Node&);

WEBCORE_EXPORT bool areRangesEqual(const Range*, const Range*);
bool rangesOverlap(const Range*, const Range*);

} // namespace

#ifndef NDEBUG
// Outside the WebCore namespace for ease of invocation from gdb.
void showTree(const WebCore::Range*);
#endif

#endif
